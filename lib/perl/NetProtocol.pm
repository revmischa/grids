package NetProtocol;
use strict;
use warnings;

use Carp;

use base qw/Class::Accessor/;
__PACKAGE__->mk_accessors(qw(encap encap_base event_handler encap_method event_handler_obj));

# autouse all encapsulation methods
use Class::Autouse;
Class::Autouse->autouse_recursive('NetProtocol::Encapsulation');

sub new {
    my ($class, %opts) = @_;

    my $enc = delete $opts{encapsulation};

    my $evt_handler = delete $opts{event_handler};
    my $handler_obj = delete $opts{event_handler_object};

    my $self = {
        event_handler => $evt_handler,
        event_handler_obj => $handler_obj,
    };

    bless $self, $class;

    if ($enc) {
        return undef unless $self->set_encapsulation_method($enc);
    }

    return $self;
}

sub set_encapsulation_method {
    my ($self, $enc) = @_;

    return undef if $enc =~ /\W/;
    my $encap_method = "NetProtocol::Encapsulation::$enc";
    my $encap = eval { $encap_method->new } or return undef;

    $self->encap_base($enc);
    $self->encap_method($encap_method);
    $self->encap($encap);

    return 1;
}

# returns a string to pass to initiate a protocol connection
sub initiation_string {
    my ($self) = @_;

    my @elements = ('Net', '1.0', $self->encap_base);
    return '==' . join('/', @elements);
}

# returns a new NetProtocol instance from an initiation string
sub new_from_initiation_string {
    my ($class, $initstr, %params) = @_;

    return undef unless $initstr =~ s/^==//;

    my ($prog, $ver, @encapsulation_classes) = split('/', $initstr);

    return undef unless $prog eq 'Net' && $ver eq '1.0' && @encapsulation_classes;

    my $p;
    # try each requested encapsulation method in listed order
    foreach my $enc (@encapsulation_classes) {
        $p = eval { $class->new(encapsulation => $enc, %params) };
        last if $p;
    }

    return $p;
}

# encapsulate a protocol event for transmission
sub encapsulate {
    my ($self, $event, $args) = @_;

    croak "Attempting to encapsulate message without an encapsulation method specified"
        unless $self->encap;

    $args ||= {};
    $args->{_method} = $event;

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

    if (index($data, '==') == 0) {
        # this is an initiation string, handle it
        my ($status, $info, $extrainfo) = $data =~ /^==(\w+)\/(\w+)(?:\/(\w*))?/;

        unless ($status) {
            warn "Got invalid request string: '$data'";
            return 0;
        }

        if ($status eq 'OK') {
            $self->set_encapsulation_method($info) or die "Invalid encapsulation method \"$info\" specified by server";
        } elsif ($status eq 'ERROR') {
            if ($info eq 'Unauthorized') {
                $self->dispatch_event_handler('Protocol.Error.Unauthorized', {message => $extrainfo});
            } elsif ($info eq 'IncompatibleVersion') {
                $self->dispatch_event_handler('Protocol.Error.IncompatibleVersion', {min_version => $extrainfo});
            } elsif ($info eq 'InvalidEncapsulations') {
                $self->dispatch_event_handler('Protocol.Error.InvalidEncapsulations');
            } else {
                $self->dispatch_event_handler('Protocol.Error.UnknownError', {error => $info});
            }
        } else {
            $self->dispatch_event_handler('Protocol.Error.UnknownStatus', {status => $status});
        }

        return;
    }

    return unless $self->event_handler;

    # decode message
    my $args = $self->decapsulate($data);
    my $event = delete $args->{_method};

    $self->dispatch_event_handler($event, $args, @extra_args);
}

sub dispatch_event_handler {
    my ($self, $event, $args, @extra) = @_;

    my $cb = $self->event_handler;

    # no handler, we don't care about anything
    return unless $cb;

    my @args = ($self, $event, $args, @extra);

    if ($self->event_handler_obj) {
        # instance method callback
        return $cb->($self->event_handler_obj, @args);
    } else {
        # class method callback
        return $cb->(@args);
    }
}
1;
