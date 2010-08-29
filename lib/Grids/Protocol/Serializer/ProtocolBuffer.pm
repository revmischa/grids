package Grids::Protocol::Serializer::ProtocolBuffer;

use Moose;
    with 'Grids::Protocol::Serializer';

use Carp qw/croak confess/;
use Google::ProtocolBuffers;
use Grids::Util;
use Grids::Protocol::Event;

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

# compile protocol definitions into classes
sub compile {
    my ($class) = @_;
    
    my $dir = $class->definitions_directory;
    Google::ProtocolBuffers->parsefile("$dir/grids.proto" => {
        include_dir => $dir,
        generate_code => Grids::Util->base_dir . "/lib/Grids/Message.pm",
        create_accessors => 1,
    });
}

no Moose;
__PACKAGE__->meta->make_immutable;
