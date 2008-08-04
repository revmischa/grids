package Grids::Protocol;
use strict;
use warnings;

use Carp qw/croak/;

use base qw/Class::Accessor/;
__PACKAGE__->mk_accessors(qw(encap encap_base encap_method id peer_identity));

use Class::Autouse qw/
    Grids::Protocol::Event
/;

# autouse all encapsulation methods
Class::Autouse->autouse_recursive('Grids::Protocol::Encapsulation');

sub new {
    my ($class, %opts) = @_;

    my $enc = delete $opts{encapsulation} || 'JSON';
    my $id = delete $opts{identity} or croak "No identity passed to Grids::Protocol::New";
    my $peer_identity = delete $opts{peer_identity};
    my $self = bless { id => $id, peer_identity => $peer_identity }, $class;

    if ($enc) {
        return undef unless $self->set_encapsulation_method($enc);
    }

    return $self;
}

sub set_encapsulation_method {
    my ($self, $enc) = @_;

    return undef if $enc =~ /\W/;
    my $encap_method = "Grids::Protocol::Encapsulation::$enc";
    my $encap = eval { $encap_method->new } or return undef;

    $self->encap_base($enc);
    $self->encap_method($encap_method);
    $self->encap($encap);

    return 1;
}

# returns a string to pass to initiate a protocol connection
sub initiation_string {
    my ($self) = @_;

    my $id = $self->id or croak "Tried to call initiation_string on a protocol with no identity defined";
    my $pubkey = $id->pubkey->serialize;

    my @elements = ('Grids', '1.0', $self->encap_base, "pubkey=\"$pubkey\"");
    return '==' . join('/', @elements);
}

# returns a new Grids::Protocol instance from an initiation string
sub new_from_initiation_string {
    my ($class, $initstr, %params) = @_;

    return undef unless $initstr =~ s/^==//;

    my ($prog, $ver, $encapsulation_classes, $pubkey) = split('/', $initstr);

    return undef unless $prog eq 'Grids' && $ver eq '1.0' && $encapsulation_classes && index($pubkey, 'pubkey=') != -1;

    ($pubkey) = $pubkey =~ m/pubkey=\"([0-9 ]+)\"/i or return undef;

    my $peer_id = Grids::Identity->new_from_serialized_pubkey($pubkey);

    my $p;
    # try each requested encapsulation method in listed order
    foreach my $enc (split(',', $encapsulation_classes)) {
        $p = eval { $class->new(encapsulation => $enc, %params, peer_identity => $peer_id) };
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

# decode a protocol transmission and return an Event record
sub parse_request {
    my ($self, $data) = @_;

    my $event;

    if (index($data, '==') == 0) {
        # this is an initiation string, handle it
        # ==Grids/1.0/JSON/pubkey=abcdefg
        my ($status, $info, $extrainfo, $pubkey) = $data =~ /^==(\w+)\/(\w+)(?:\/(\w*))?(?:\/pubkey="([\w\s]+)")?/i;

        unless ($status) {
            warn "Got invalid request string: '$data'";
            return undef;
        }

        if ($status eq 'OK') {
            $self->set_encapsulation_method($info) or die "Invalid encapsulation method \"$info\" specified by server";
            return $self->event('ProtocolEstablished', {pubkey => $pubkey});
        } elsif ($status eq 'ERROR') {
            if ($info eq 'Unauthorized') {
                return $self->error_event('Error.Protocol.Unauthorized', {message => $extrainfo});
            } elsif ($info eq 'IncompatibleVersion') {
                return $self->error_event('Error.Protocol.IncompatibleVersion', {min_version => $extrainfo});
            } elsif ($info eq 'InvalidEncapsulations') {
                return $self->error_event('Error.Protocol.InvalidEncapsulations');
            } else {
                return $self->error_event('Error.Protocol.UnknownError', {msg => $info});
            }
        } else {
            return $self->error_event('Error.Protocol.UnknownStatus', {status => $status});
        }
    }

    # decode message
    my $args = $self->decapsulate($data);

    # instantiate Event record
    my $event_name = delete $args->{_method};
    return Grids::Protocol::Event->new(event_name => $event_name, params => $args);
}

sub error_event {
    my ($self, $error_event, $params) = @_;
    $params ||= {};
    $params->{error} = 1;
    return Grids::Protocol::Event->new(event_name => $error_event, params => $params);
}

sub event {
    my ($self, $event, $params) = @_;
    return Grids::Protocol::Event->new(event_name => $event, params => $params);
}

1;
