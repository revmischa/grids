package NetProtocol;
use strict;
use warnings;

use Carp;
use base qw/Class::Accessor/;

__PACKAGE__->mk_accessors(qw(encap event_handler encap_method));

# autouse all encapsulation methods
use Class::Autouse;
Class::Autouse->autouse_recursive('NetProtocol::Encapsulation');

sub new {
    my ($class, %opts) = @_;

    my $enc = delete $opts{encapsulation};
    my $evt_handler = delete $opts{event_handler};

    my $encap_method = "NetProtocol::Encapsulation::$enc";
    my $encap = $encap_method->new;

    my $self = {
        encap_method => $enc,
        encap => $encap,
        event_handler => $evt_handler,
    };

    bless $self, $class;
    return $self;
}

# returns a string to pass to initiate a protocol connection
sub initiation_string {
    my ($self) = @_;

    return join(':', ($self->encap_method));
}

# returns a new NetProtocol instance from an initiation string
sub new_from_initiation_string {
    my ($class, $initstr, %params) = @_;

    my ($enc) = split(':', $initstr);

    return undef unless $enc;

    my $p = $class->new(encapsulation => $enc, %params);
    return $p;
}

# encapsulate a protocol event for transmission
sub encapsulate {
    my ($self, $event, $args) = @_;

    croak "Attempting to encapsulate message without an encapsulation method specified"
        unless $self->encap;

    $args ||= {};
    $args->{_event} = $event;

    return $self->encap->encapsulate($args);
}

# should never need to be called directly
sub decapsulate {
    my ($self, $data) = @_;

    croak "Attempting to decapsulate message without an encapsulation method specified"
        unless $self->encap;

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
