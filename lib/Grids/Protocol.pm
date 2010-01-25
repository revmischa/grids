package Grids::Protocol;
use strict;
use warnings;

use Carp qw/croak/;

use base qw/Class::Accessor/;
__PACKAGE__->mk_accessors(qw(encap encap_base encap_method id peer));

use Class::Autouse qw/
    Grids::Protocol::Event
    Grids::Peer
/;

# autouse all encapsulation methods
Class::Autouse->autouse_recursive('Grids::Protocol::Encapsulation');

use constant {
    MSG_INIT_PROTOCOL_PREFIX => '++',
    MSG_INIT_PROTOCOL_REPLY_PREFIX => '==',
};

sub new {
    my ($class, %opts) = @_;

    my $enc = delete $opts{encapsulation} || 'JSON';
    my $id = delete $opts{identity} or croak "No identity passed to Grids::Protocol::New";
    my $peer = delete $opts{peer};
    my $self = bless { id => $id, peer => $peer }, $class;

    if ($enc) {
        return undef unless $self->set_encapsulation_method($enc);
    }

    return $self;
}

sub set_encapsulation_method {
    my ($self, $enc) = @_;

    return undef if $enc !~ /^[\w:]+$/;
    my $encap_method = "Grids::Protocol::Encapsulation::$enc";
    my $encap = eval { $encap_method->new };

    if (! $encap || $@) {
        die "Failed to create protocol handler: $@\n";
    }

    $self->encap_base($enc);
    $self->encap_method($encap_method);
    $self->encap($encap);

    return 1;
}

# returns a string to pass to initiate a protocol connection
sub initiation_string {
    my ($self) = @_;

    my $id = $self->id or croak "Tried to call initiation_string on a protocol with no identity defined";
    my $name = $id->name;

    my @elements = ('Grids', '1.0', $self->encap_base, "name=\"$name\"");
    return MSG_INIT_PROTOCOL_PREFIX . join('/', @elements);
}

# return bob@node_name to uniquely identify this user
sub peer_name {
    my ($self) = @_;
    return $self->peer->name . '@' . $self->id->name;
}

# returns a string to respond to a protocol initiation
sub protocol_init_response {
    my ($self, %opts) = @_;

    my $id = $self->id or croak "Tried to call protocol_init_response on a protocol with no identity defined";
    my $name = $id->name;
    
    my @elements = ('OK', '1.0', $self->encap_base, "name=\"$name\"");
    return MSG_INIT_PROTOCOL_REPLY_PREFIX . join('/', @elements);
}

sub establish_encrypted_connection {
    my ($self) = @_;

    croak "establish_encrypted_connection() called but no peer defined"
        unless $self->peer;

    $self->id->otr->establish($self->peer_name);
}

sub set_peer_name {
    my ($self, $name) = @_;

    my $peer = $self->{peer};
    if ($peer) {
        $peer->{name} = $name;
    } else {
        $peer = new Grids::Peer(name => $name);
        $self->{peer} = $peer;
    }
}

# returns a new Grids::Protocol instance from an initiation string
sub new_from_initiation_string {
    my ($class, $initstr, %params) = @_;

    my $prefix = MSG_INIT_PROTOCOL_PREFIX;
    return undef unless index($initstr, $prefix) == 0;
    $initstr = substr($initstr, length $prefix);

    my ($prog, $ver, $encapsulation_classes, $name) = split('/', $initstr);

    return undef unless $prog eq 'Grids' && $ver eq '1.0' && $encapsulation_classes;

    if ($name && index($name, 'name=') != -1) {
        ($name) = $name =~ m/name=\"([-\w]+)\"/i or return undef;

        my $peer = new Grids::Peer(name => $name);
        $params{peer} = $peer;
    } else {
        warn "did not get peer name";
    }

    my $p;
    # try each requested encapsulation method in listed order
    foreach my $enc (split(',', $encapsulation_classes)) {
        $p = eval { $class->new(encapsulation => $enc, %params) };
        last if $p;
    }

    return $p;
}

# decode a protocol transmission and return an Event record
sub parse_request {
    my ($self, $data) = @_;

    my $event;

    my $prefix = MSG_INIT_PROTOCOL_REPLY_PREFIX;
    if (index($data, $prefix) == 0) {
        # this is an initiation reply string, handle it

        # ==OK/1.0/JSON/name=$name
        $data = substr($data, length $prefix);   # chop prefix off
        
        # parse reply
        my ($status, $version, $info, $name) = $data =~ m{
              ^(\w+)                    # Status (OK/ERROR)
              \/([^\/]+)                # Version
              (?:\/(\w*))?              # Encapsulation method (e.g. JSON, XML)
              (?:\/name="([-\w\s]+)")?  # options, currently only name is supported
        }smix;

        unless ($status) {
            warn "Got invalid request string: '$data'";
            return undef;
        }

        if ($version ne '1.0') {
            warn "Warning: peer is using an unknown version of Grids, things may break\n";
        }

        if ($status eq 'OK') {
            # we got a protocol response, we have their name and encapsulation method
            $self->set_peer_name($name);

            # try to create encapsulation subtype
            $self->set_encapsulation_method($info)
                or return $self->error_event('Error.Protocol.UnsupportedEncapsulation', {encapsulation_method => $info});

            # send request to establish an encrypted session
            $self->establish_encrypted_connection;

            # protocol is set up, we are ready to send events but we
            # should wait until we have an encrypted session
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
    my ($args, $was_encrypted) = eval { ($self->decapsulate($data)) };

    if (! $args) {
        warn "Could not decapsulate protocol message: $@" if $@;
        return undef;
    }

    warn "Received message from " . $self->peer_name . " but it was not encrypted: $data\n" unless $was_encrypted;

    # instantiate Event record
    my $event_name = delete $args->{_method};
    return Grids::Protocol::Event->new(event_name => $event_name, params => $args, was_encrypted => $was_encrypted);
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

# take a received message string and parse it into a native data structure
# should never need to be called directly
# in list context also returns if the message was encrypted
sub decapsulate {
    my ($self, $data) = @_;

    croak "Attempting to decapsulate message without an encapsulation method specified"
        unless $self->encap;

    my $was_encrypted;
    if ($self->peer && $self->peer_name) {
        my $decrypted;
        ($decrypted, $was_encrypted) = $self->decrypt_message($data);

        if ($was_encrypted) {
            $data = $decrypted if defined $decrypted;
        } else {
            return wantarray ? (undef, 0) : undef;
        }
    } else {
        warn "no peer defined, unable to decrypt message";
    }

    my $args = $self->encap->decapsulate($data);
    return wantarray ? ($args, $was_encrypted) : $args;
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
    $msg = $self->encrypt_message($msg) if $self->peer;

    return $msg;
}


# encrypt this message for our peer, if we have their public key
sub encrypt_message {
    my ($self, $msg) = @_;

    croak "encrypt_message() called without a peer defined"
        unless $self->peer && $self->peer_name;

    my $ciphertext = $self->id->encrypt($self->peer_name, $msg);
    if ($ciphertext) {
        return $ciphertext;
    }

    return $msg;
}

# takes a message and decrypts it using our privkey
# returns ($message, $was_decrypted)
sub decrypt_message {
    my ($self, $msg) = @_;

    croak "decrypt_message() called without a peer defined"
        unless $self->peer && $self->peer_name;

    my $plaintext = $self->id->decrypt($self->peer_name, $msg);
    if (defined $plaintext) {
        return wantarray ? ($plaintext, 1) : $plaintext;
    } else {
        return wantarray ? ($msg, 0) : $msg;
    }
}







# methods below are busted 


sub set_peer_pubkey {
    my ($self, %opts) = @_;

    # only supports serialized pubkey for now
    my $serialized = $opts{serialized_pubkey} or return undef;

    my $peer_id = $self->deserialize_pubkey($serialized);
    my $peer = new Grids::Peer(id => $peer_id);
    $self->{peer} = $peer;
}

sub deserialize_pubkey {
    my ($class, $pubkey) = @_;

    my $id = Grids::Identity->new_from_serialized_pubkey($pubkey);
    return $id;
}

1;
