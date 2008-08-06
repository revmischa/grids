package Grids::Protocol;
use strict;
use warnings;

use Carp qw/croak/;

use base qw/Class::Accessor/;
__PACKAGE__->mk_accessors(qw(encap encap_base encap_method id peer_id));

use Class::Autouse qw/
    Grids::Protocol::Event
/;

# autouse all encapsulation methods
Class::Autouse->autouse_recursive('Grids::Protocol::Encapsulation');

use constant {
    MSG_ENCRYPTED_PREFIX => '--',
    MSG_PLAINTEXT_PREFIX => '==',
};

sub new {
    my ($class, %opts) = @_;

    my $enc = delete $opts{encapsulation} || 'JSON';
    my $id = delete $opts{identity} or croak "No identity passed to Grids::Protocol::New";
    my $peer_id = delete $opts{peer_id};
    my $self = bless { id => $id, peer_id => $peer_id }, $class;

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
    return MSG_PLAINTEXT_PREFIX . join('/', @elements);
}

# returns a string to respond to a protocol initiation
sub protocol_init_response {
    my ($self, %opts) = @_;

    my $id = $self->id or croak "Tried to call protocol_init_response on a protocol with no identity defined";
    my $pubkey = $id->pubkey->serialize;
    
    my @elements = ('OK', '1.0', $self->encap_base, "pubkey=\"$pubkey\"");
    return MSG_PLAINTEXT_PREFIX . join('/', @elements);
}

sub set_peer_pubkey {
    my ($self, %opts) = @_;

    # only supports serialized pubkey for now
    my $serialized = $opts{serialized_pubkey} or return undef;

    my $peer_id = $self->deserialize_pubkey($serialized);
    $self->{peer_id} = $peer_id;
}

sub deserialize_pubkey {
    my ($class, $pubkey) = @_;

    my $id = Grids::Identity->new_from_serialized_pubkey($pubkey);
    return $id;
}

# returns a new Grids::Protocol instance from an initiation string
sub new_from_initiation_string {
    my ($class, $initstr, %params) = @_;

    my $prefix = MSG_PLAINTEXT_PREFIX;
    return undef unless $initstr =~ s/^$prefix//;

    my ($prog, $ver, $encapsulation_classes, $pubkey) = split('/', $initstr);

    return undef unless $prog eq 'Grids' && $ver eq '1.0' && $encapsulation_classes;

    if ($pubkey && index($pubkey, 'pubkey=') != -1) {
        ($pubkey) = $pubkey =~ m/pubkey=\"([0-9 ]+)\"/i or return undef;

        my $peer_id = $class->deserialize_pubkey($pubkey);
        $params{peer_id} = $peer_id;
    }

    my $p;
    # try each requested encapsulation method in listed order
    foreach my $enc (split(',', $encapsulation_classes)) {
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

    my $msg = $self->encap->encapsulate($args);

    # do we have a public key for the other party? if so, encrypt this message for them
    $msg = $self->encrypt_message($msg);

    return $msg;
}

# encrypt this message for our peer, if we have their public key
sub encrypt_message {
    my ($self, $msg) = @_;

    my $peer_id = $self->peer_id;
    return $msg unless $peer_id && $self->id;

    return MSG_ENCRYPTED_PREFIX . $self->id->encrypt($msg, $peer_id);
}

# takes a message and decrypts it using our privkey
sub decrypt_message {
    my ($self, $msg_orig) = @_;

    my $msg = $msg_orig;

    # is this message encrypted?
    if (index($msg_orig, MSG_ENCRYPTED_PREFIX) == 0) {
        $msg = substr($msg_orig, 2);

        my $privkey = $self->id->privkey;
        unless ($privkey) {
            warn "No privkey set on protocol receiving encrypted message, cannot decrypt message";
            return $msg;
        }

        # decrypt it
        my $decrypted = $self->id->decrypt($msg);

        unless ($decrypted) {
            warn "Could not decrypt message";
            return $msg;
        }

        $msg = $decrypted;
    }

    return $msg;
}

# take a received message string and parse it into a native data structure
# should never need to be called directly
sub decapsulate {
    my ($self, $data) = @_;

    croak "Attempting to decapsulate message without an encapsulation method specified"
        unless $self->encap;

    $data = $self->decrypt_message($data);
    my $args = $self->encap->decapsulate($data);
    return $args;
}

# decode a protocol transmission and return an Event record
sub parse_request {
    my ($self, $data) = @_;

    my $event;

    my $prefix = MSG_PLAINTEXT_PREFIX;
    if (index($data, $prefix) == 0) {
        # this is an initiation string, handle it
        # ==Grids/1.0/JSON/pubkey=bitsize e n ident
        my ($status, $version, $info, $pubkey) = $data =~ /^$prefix(\w+)\/([^\/]+)(?:\/(\w*))?(?:\/pubkey="([\w\s]+)")?/i;

        unless ($status) {
            warn "Got invalid request string: '$data'";
            return undef;
        }

        if ($status eq 'OK') {
            $self->set_peer_pubkey(serialized => $pubkey);

            $self->set_encapsulation_method($info)
                or return $self->error_event('Error.Protocol.UnsupportedEncapsulation', {encapsulation_method => $info});

            return $self->event('ProtocolEstablished');
        } elsif ($status eq 'ERROR') {
            if ($info eq 'Unauthorized') {
                return $self->error_event('Error.Protocol.Unauthorized', {message => $info});
            } elsif ($info eq 'IncompatibleVersion') {
                return $self->error_event('Error.Protocol.IncompatibleVersion', {min_version => $version});
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

    unless ($args) {
        warn "Could not decapsulate protocol message";
        return undef;
    }

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
