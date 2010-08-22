package Grids::Protocol;

use Moose;
use Carp qw/croak/;

has serializer => (
    is => 'rw',
    does => 'Grids::Protocol::Serializer',
);

has serializer_class => (
    is => 'rw',
    isa => 'Str',
    default => sub { 'ProtocolBuffer' },
);

has serializer_method => (
    is => 'rw',
    isa => 'Str',
);

has id => (
    is => 'rw',
    isa => 'Grids::Identity',
    required => 1,
    handles => [qw/initiate_smp/],
);

has peer => (
    is => 'rw',
    isa => 'Grids::Peer',
    predicate => 'has_peer',
);

has use_encryption => (
    is => 'rw',
    isa => 'Bool',
    default => sub { 1 },
);

has encrypted_connection_started => (
    is => 'rw',
    isa => 'Bool',
    default => sub { 0 },
);

has got_initiation => (
    is => 'rw',
    isa => 'Bool',
    default => sub { 0 },
);

use Grids::Message;
use Grids::Peer;
use Grids::Protocol::Event;
use Grids::Protocol::Serializer;
use Grids::Protocol::Serializer::JSON;
use Grids::Protocol::Serializer::ProtocolBuffer;

use constant {
    MSG_INIT_PROTOCOL_PREFIX => '++',
    MSG_INIT_PROTOCOL_REPLY_PREFIX => '==',
};

sub BUILD {
    my $self = shift;

    my $ser = $self->serializer_class;

    $self->set_serializer_method($ser)
        or die "Unable to load serialization class $ser";
}

sub set_serializer_method {
    my ($self, $ser) = @_;

    # fixme: use MooseX::Traits
    return undef if $ser !~ /^[\w:]+$/;
    my $ser_method = "Grids::Protocol::Serializer::$ser";
    my $serializer = eval { $ser_method->new };

    if (! $serializer || $@) {
        die "Failed to create protocol handler: $@\n";
    }

    $self->serializer_class($ser);
    $self->serializer_method($ser_method);
    $self->serializer($serializer);

    return 1;
}

# returns a string to pass to initiate a protocol connection
sub initiation_string {
    my ($self) = @_;

    my $id = $self->id or croak "Tried to call initiation_string on a protocol with no identity defined";
    my $name = $id->name;

    my @elements = ('Grids', '1.0', $self->serializer_class, "name=\"$name\"");
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
    
    my @elements = ('OK', '1.0', $self->serializer_class, "name=\"$name\"");
    return MSG_INIT_PROTOCOL_REPLY_PREFIX . join('/', @elements);
}

sub establish_encrypted_connection {
    my ($self) = @_;

    croak "establish_encrypted_connection() called but no peer defined"
        unless $self->peer;

    $self->use_encryption(1);

    $self->id->otr->establish($self->peer_name);
    $self->encrypted_connection_started(1);
}

sub end_encrypted_connection {
    my ($self) = @_;

    croak "end_encrypted_connection() called but no peer defined"
        unless $self->peer;

    $self->id->otr->finish($self->peer_name)
        if $self->encrypted_connection_started;

    $self->encrypted_connection_started(0);
}

sub set_peer_name {
    my ($self, $name) = @_;

    my $peer = $self->peer or croak "set_peer_name called but no we have no peer object";
    $peer->name($name);
}

# returns a new Grids::Protocol instance from an initiation string
sub new_from_initiation_string {
    my ($class, $initstr, $connection, $params) = @_;

    croak "No connection passed to new_from_initiation_string()" unless $connection;
    $params ||= {};

    my $prefix = MSG_INIT_PROTOCOL_PREFIX;
    return unless index($initstr, $prefix) == 0;
    $initstr = substr($initstr, length $prefix);

    my ($prog, $ver, $serializer_classes, $name) = split('/', $initstr);

    return unless $prog eq 'Grids' && $ver eq '1.0' && $serializer_classes;

    my $peer;
    if ($name && index($name, 'name=') != -1) {
        ($name) = $name =~ m/name=\"([-\w]+)\"/i or return;

        $peer = new Grids::Peer(connection => $connection, name => $name);
        $params->{peer} = $peer;
    } else {
        warn "did not get peer name";
        return;
    }

    my $p;
    # try each requested serializer method in listed order
    foreach my $ser (split(',', $serializer_classes)) {
        $p = eval { $class->new(serializer_class => $ser, %$params) };
        warn $@ if $@;
        last if $p;
    }

    unless ($p) {
        warn "unable to load suitable serializer for $serializer_classes";
        return;
    }

    $p->peer($peer);
    $p->got_initiation(1);

    $connection->protocol($p);
    $connection->transport->connection_ready($connection);

    return $p;
}

# decode a protocol transmission and return an Event record
sub parse_request {
    my ($self, $connection, $data) = @_;

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
              (?:\/(\w*))?              # Serializer method (e.g. JSON, ProtocolBuffer)
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
            # we got a protocol response, we have their name and serializer method

            # create peer object
            my $peer = Grids::Peer->new(name => $name, connection => $connection);
            $self->peer($peer);

            # try to create serializer subtype
            $self->set_serializer_method($info)
                or return $self->error_event('Error.Protocol.UnsupportedSerializer', $connection, {serializer_method => $info});

            # send request to establish an encrypted session
            $self->establish_encrypted_connection if $self->use_encryption;

            # protocol is set up, we are ready to send events but we
            # should wait until we have an encrypted session

            # inform the transport we are ready to send/receive events
            $connection->transport->connection_ready($connection);

            # post a Connected event
            return $self->event('Connected', $connection, { peer_name => $self->id->name });
        } elsif ($status eq 'ERROR') {
            if ($info eq 'Unauthorized') {
                return $self->error_event('Error.Protocol.Unauthorized', $connection, {message => $info});
            } elsif ($info eq 'IncompatibleVersion') {
                return $self->error_event('Error.Protocol.IncompatibleVersion', $connection, {min_version => $version});
            } elsif ($info eq 'InvalidSerializer') {
                return $self->error_event('Error.Protocol.InvalidSerializer', $connection);
            } else {
                return $self->error_event('Error.Protocol.UnknownError', $connection, {msg => $info});
            }
        } else {
            return $self->error_event('Error.Protocol.UnknownStatus', $connection, {status => $status});
        }
    }

    # decode message
    my ($evt, $was_encrypted) = eval { ($self->deserialize_event($data)) };

    if (! $evt) {
        warn "Could not deserialize protocol message: $@" if $@;
        return undef;
    }

    warn "Received message from " . $self->peer_name . " but it was not encrypted: $data\n"
        if ! $was_encrypted && $self->use_encryption;

    $evt->connection($connection);
    $evt->was_encrypted($was_encrypted);

    return $evt;
}

sub error_event {
    my ($self, $error_event, $connection, $params) = @_;
    $params ||= {};
    $params->{error} = 1;
    return $self->event($error_event, $connection, $params);
}

sub event {
    my ($self, $event, $connection, $params) = @_;
    return Grids::Protocol::Event->new(event_name => $event, args => $params, connection => $connection);
}

# take a received message string and parse it into an event instance
# should never need to be called directly
# in list context also returns if the message was encrypted
sub deserialize_event {
    my ($self, $data) = @_;

    croak "Attempting to deserialize event without a serialization method specified"
        unless $self->serializer;

    my $was_encrypted;
    if ($self->use_encryption && $self->peer && $self->peer_name) {
        my $decrypted;
        ($decrypted, $was_encrypted) = $self->decrypt_message($data);

        if ($was_encrypted) {
            $data = $decrypted if defined $decrypted;
        } else {
            return wantarray ? (undef, 0) : undef;
        }
    } else {
        warn "no peer defined, unable to decrypt message" if $self->use_encryption;
    }

    my $event = $self->serializer->deserialize($data);
    return wantarray ? ($event, $was_encrypted) : $event;
}

# serialize a protocol event for transmission
# takes either Event instance or ($event_name, $args_hashref)
sub serialize_event {
    my ($self, $event, $args) = @_;

    croak "Attempting to serialize event without a serialization method specified"
        unless $self->serializer;

    my $msg = eval {
        $event = $self->serializer->construct_event($event, $args);
        return $self->serializer->serialize($event);
    };
    unless ($msg) {
        warn "Failed to serialize event $event: $@";
        return;
    }

    # do we have a public key for the other party? if so, encrypt this message for them
    if ($self->peer && $self->use_encryption) {
        $msg = $self->encrypt_message($msg);
    }

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

no Moose;
__PACKAGE__->meta->make_immutable;

