package NetProtocol;
use strict;
use warnings;

use Carp;
use base qw/Class::Accessor/;

__PACKAGE__->mk_accessors(qw(encap event_handler));

# autouse all encapsulation methods
use Class::Autouse;
Class::Autouse->autouse_recursive('NetProtocol::Encapsulation');

sub new {
    my ($class, %opts) = @_;

    my $enc = delete $opts{encapsulation} or croak "No encapsulation method specified";
    my $evt_handler = delete $opts{event_handler};

    my $encap_class = "NetProtocol::Encapsulation::$enc";
    my $encap = $encap_class->new;

    my $self = {
        encap => $encap,
        event_handler => $evt_handler,
    };

    bless $self, $class;
    return $self;
}

# encapsulate a protocol event for transmission
sub encapsulate {
    my ($self, $event, $args) = @_;

    $args ||= {};
    $args->{_event} = $event;

    return $self->encap->encapsulate($args);
}

# should never need to be called directly
sub decapsulate {
    my ($self, $data) = @_;
    my $args = $self->encap->decapsulate($data);
    return $args;
}

# decode a protocol transmission and call event handler
sub handle_request {
    my ($self, $data) = @_;

    my $cb = $self->event_handler;

    # no handler, we don't care about anything
    return unless $cb;

    # decode message
    my $args = $self->decapsulate($data);
    my $event = delete $args->{_event};

    # call event handler
    return $cb->($self, $event, $args);
}

1;
