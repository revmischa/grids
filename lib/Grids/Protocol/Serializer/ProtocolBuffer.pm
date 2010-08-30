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

    foreach my $file (@files) {
        # transform proto filename into class name
        my ($name) = $file =~ /\/?(\w+).proto$/i;
        croak "I don't understand the filename $file" unless $name;
        $name = lcfirst $name;

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
}

no Moose;
__PACKAGE__->meta->make_immutable;
