package Grids::Protocol::Serializer::ProtocolBuffer;

use Moose;
    with 'Grids::Protocol::Serializer';

use Carp qw/croak confess/;
use Google::ProtocolBuffers;
use Grids::Util;

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
    
    my $event_name = $event->event
        or croak "Trying to serialize event with no event name";
        
    my $evt_class = $self->get_event_class($event_name);
    
    # maybe this should just be a warning?
    eval "use $evt_class; 1"
        or croak "Attempting to transmit event of type $event_name but no protocol definition exists.";
        
    my $msg_str = eval {
        $evt_class->encode($event->args || {}); 
    } or croak "Unable to serialize event $event_name: $@";
    
    return "$evt_class|$msg_str";
}

sub deserialize {
    my ($self, $data) = @_;
    
    my ($event_class, $msg_str) = $data =~ /^([\w:]+\|(.*)$)/sm;
    $event_class = eval { $self->get_event_class($event_class); };
    unless ($event_class) {
        warn $@;
        return;
    }
    
    my $evt = eval { $event_class->decode($data); };
    unless ($evt) {
        warn $@;
        return;
    }
    
    return $evt;
}

# turn "Foo.Bar" or "Foo::Bar" into "Grids::Protocol::Message::Event::Foo::Bar"
sub get_event_class {
    my ($self, $event_class) = @_;
    
    confess "Invalid event class $event_class"
        unless $event_class =~ /^[\w:\.]+$/sm;
    
    # find compiled event subclass
    $event_class =~ s/\./::/g;
    $event_class = "Event::$event_class" unless $event_class =~ /^Event::/;
    $event_class = "Grids::Protocol::Message::$event_class";
    
    # maybe this should just be a warning?
    eval "use $event_class; 1"
        or confess "Attempting to handle event of type $event_class but no protocol definition exists.";

    return $event_class;
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
        generate_code => Grids::Util->base_dir . "/lib/Grids/Protocol/Message/Event.pm",
        create_accessors => 1,
    });
}

no Moose;
__PACKAGE__->meta->make_immutable;
