package Grids::Protocol::Serializer::ProtocolBuffer;

use Moose;
    with 'Grids::Protocol::Serializer';

use Carp qw/croak confess/;
use Google::ProtocolBuffers;
use Grids::Util;
use Grids::Protocol::Event;
use File::Temp qw/tempfile/;

has parser => (
    is => 'rw',
    lazy => 1,
    builder => 'build_parser',
);

sub build_parser {
    my ($self) = @_;
    
    # Google::ProtocolBuffers interface is just class methods, not OO
    return 'Google::ProtocolBuffers';
}

sub serialize {
    my ($self, $event) = @_;
    
    my $event_name = $event->name
        or confess "Trying to serialize event with no event name";

    # get event as a hashref and serialize
    my $msg_str = eval {
        my $ser = $event->serialize;

        # event name is encoded in the serialized transmission. eliminate it to save space
        # we need to copy base so we don't mess up the original event
        my %new_base = %{$ser->{base}};
        delete $new_base{event};
        $ser->{base} = \%new_base;

        $event->encode($ser); 
    } or confess "Unable to serialize event $event_name: $@";
    
    return "$event_name\x01$msg_str";
}

sub deserialize {
    my ($self, $data) = @_;
    
    my ($event_name, $msg_str) = $data =~ /^([\.\w:]+)\x01(.*)$/sm;

    unless ($event_name && $msg_str) {
        warn "Failed to parse message: '$data'";
        return;
    }

    my $message_class = eval { $self->get_message_class($event_name); };
    unless ($message_class) {
        warn $@;
        return;
    }
    
    my $evt = eval { $message_class->decode($msg_str); };
    unless ($evt) {
        warn "Failed to parse message: '$data': $@";
        return;
    }

    $evt->base->{event} = $event_name;

    # apply Event role
    Grids::Protocol::Event->meta->apply($evt);
    
    return $evt;
}

# path to protocol definitions
sub definitions_directory {
    my $base_dir = Grids::Util->base_dir;
    return $base_dir . '/protocol';
}

# compile protocol definition into a class file
sub compile {
    my ($class, @files) = @_;

    my $out = '';
    my %packages;

    foreach my $file (@files) {
        # read in file, get message definitions
        my $infh;
        my $in;
        open $infh, $file or die $!;
        { local $/; $in = <$infh>; }
        close $infh;
        my (@classes) = $in =~ /message\s+(\w+)\s*\{/g;

        # transform proto filename into class name
        my ($name) = $file =~ /\/?(\w+).proto$/i;
        croak "I don't understand the filename $file" unless $name;
        $packages{$name} = \@classes;
        $name = ucfirst $name;

        my $dir = $class->definitions_directory;

        my ($outfh, $outfile) = tempfile();
        Google::ProtocolBuffers->parsefile($file => {
            include_dir => $dir,
            generate_code => $outfh,
            create_accessors => 1,
        });
        close $outfh;

        open my $outfilefh, $outfile or die $!;
        {
            local $/;
            $out .= <$outfilefh>;
        }
        close $outfilefh;
    }

    # save classes
    my $save = Grids::Util->base_dir . "/lib/Grids/Message.pm";
    open my $savefh, ">", $save or die $!;
    print $savefh $out;
    close $savefh;

    # generate awesome C++ switch statement for instantiation
    my $cases = "if (false) {}\n";
    foreach my $pkg (keys %packages) {
        next if $pkg eq 'base';

        my $full_pkg = "grids::message";
        $full_pkg .= "::$pkg" if $pkg ne 'grids';

        my @classes = @{ $packages{$pkg} };

        my @cases = map {
            my $type = "${full_pkg}::$_";
            "else if (type == \"$type\")\n{\n  " .
                "msg = new $type();\n  msg->ParseFromString(in_msg);\n}" 
        } @classes;

        $cases .= join("\n", @cases);
    }

    my $cases_file = Grids::Util->base_dir . "/src/protocol_type_parser.cpp";
    open my $cases_file_fh, ">", $cases_file or die $!;
    print $cases_file_fh $cases;
    close $cases_file_fh;
}

no Moose;
__PACKAGE__->meta->make_immutable;
