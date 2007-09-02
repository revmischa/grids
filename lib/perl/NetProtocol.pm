package NetProtocol;
use strict;
use warnings;

use Carp;

use base qw/Class::Accessor/;
__PACKAGE__->mk_accessors(qw(encap event_handler encap_method event_handler_obj));

# autouse all encapsulation methods
use Class::Autouse;
Class::Autouse->autouse_recursive('NetProtocol::Encapsulation');

sub new {
    my ($class, %opts) = @_;

    my $enc = delete $opts{encapsulation};

    my $evt_handler = delete $opts{event_handler};
    my $handler_obj = delete $opts{event_handler_object};

    return undef if $enc =~ /\W/;
    my $encap_method = "NetProtocol::Encapsulation::$enc";
    my $encap = eval { $encap_method->new } or return undef;

    my $self = {
        encap_method => $enc,
        encap => $encap,
        event_handler => $evt_handler,
        event_handler_obj => $handler_obj,
    };

    bless $self, $class;
    return $self;
}

# returns a string to pass to initiate a protocol connection
sub initiation_string {
    my ($self) = @_;

    return join('/', ($self->encap_method));
}

# returns a new NetProtocol instance from an initiation string
sub new_from_initiation_string {
    my ($class, $initstr, %params) = @_;

    my ($enc) = split('/', $initstr);

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
    my ($self, $data, @extra_args) = @_;

    my $cb = $self->event_handler;

    # no handler, we don't care about anything
    return unless $cb;

    # decode message
    my $args = $self->decapsulate($data);
    my $event = delete $args->{_event};

    # call event handler
    {
        my @args = ($self, $event, $args, @extra_args);

        if ($self->event_handler_obj) {
            # instance method callback
            return $cb->($self->event_handler_obj, @args);
        } else {
            # class method callback
            return $cb->(@args);
        }
    }
}

1;
