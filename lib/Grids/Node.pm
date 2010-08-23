# This is a class for a Grids Node.  
# It handles sending/receiving data over transports and handling requests

package Grids::Node;

use Moose;
    with 'Grids::Base';
    with 'Grids::Transport::Consumer';  # receives transport-related callbacks

use Class::Autouse qw/
    Grids::Node::ClientStorage
    Grids::Code
    Grids::VM
    Grids::Conf
    Grids::Protocol
    Grids::Protocol::Event
    Grids::Protocol::EventQueue
    Grids::Network
/;

use Carp qw/croak/;

has 'network_id' => (
    is => 'rw',
    isa => 'Grids::Identity',
    lazy => 1,
    builder => 'id',  # we alone are the network by default
);

has 'network' => (
    is => 'rw',
    isa => 'Grids::Network',
    lazy => 1,
    builder => '_network_builder',
    handles => [qw/all_peers all_connections all_protocols/],
);

has 'client_storage' => (
    is => 'rw',
    isa => 'Grids::Node::ClientStorage',
    lazy => 1,
    builder => '_client_storage_builder',
    handles => [qw//],
);

after activate_encryption => sub {
    my ($self) = @_;

    $self->enable_encryption;
    foreach my $p ($self->all_protocols) {
        $p->establish_encrypted_connection;
    }
};

after deactivate_encryption => sub {
    my ($self) = @_;

    $self->disable_encryption;
    foreach my $p ($self->all_protocols) {
        $p->end_encrypted_connection;
    }
};

after enable_encryption => sub {
    my ($self) = @_;
    my @protocols = $self->all_protocols;
    foreach my $p (@protocols) {
        $p->use_encryption(1);
        $self->log->info("encryption enabled for " . $p->peer_name);
    }
};

after disable_encryption => sub {
    my ($self) = @_;
    my @protocols = $self->all_protocols;
    foreach my $p (@protocols) {
        $p->use_encryption(0);
        $self->log->info("encryption disabled for " . $p->peer_name);
    }
};

__PACKAGE__->load_hooks;

#############

# create new network instance
sub _network_builder {
    my ($self) = @_;
    return new Grids::Network(node => $self);
}

# create store for client data
sub _client_storage_builder {
    my ($self) = @_;
    return Grids::Node::ClientStorage->new(node => $self);
}

# send an event to all nodes in this network
sub network_broadcast {
    my ($self, $event) = @_;

    my $network = $self->network;
    $network->send_to_peers($event);
}

# someone has connected to us
sub incoming_connection_established {
    my ($self, $connection) = @_;
    $self->log->debug("inbound connection established on transport " . $connection->transport);
}

# we have connected to a server
sub outgoing_connection_established {
    my ($self, $connection) = @_;
    $self->log->debug("outbound connection established on transport " . $connection->transport);
    $self->initiate_node_protocol($connection);
}

# ready to send/receive events to a peer
sub connection_ready {
    my ($self, $connection) = @_;
    
    $self->log->debug("node connection ready with peer " . $connection->peer->name);

    # inbound connections have already been added
    $self->network->add_to_peers(peer => $connection->peer)
        if $connection->outbound;
}

# actually disconnected
sub disconnected {
    my ($self, $connection) = @_;

    # remove peer connection
    # Removed if ($connection->outbound)
    # Change by Patrick 4.14.10 due to network message duplication

    if ($connection->has_protocol && $connection->has_peer) {
        my $ok = $self->network->remove_from_peers($connection->peer);
        $self->log->warn("network->remove_from_peers failed for " . $connection->peer->name) unless $ok;
    }

    # don't really need to do anything unless protocol has been established already
    return unless $connection->has_protocol;

    if (! $connection->has_peer || ! $self->network->peer_sessions($connection->peer)) {
        $self->log->warn("got disconnected but no connection was established");
        return;
    }

    $connection->teardown_protocol;
}

# called when a connection to another node is established to set up a
# protocol communication layer with the other node
sub initiate_node_protocol {
    my ($self, $connection) = @_;

    croak "Trying to initiate grids protocol on a node with no identity object set" unless $self->id;

    $connection->initiate_protocol(
        id => $self->id,
        use_encryption => $self->use_encryption,
    );
}

sub data_received {
    my ($self, $connection, $data) = @_;

    # todo: make sure this is destroyed when connection is closed
    my $protocol_handler = $connection->protocol;

    if ($protocol_handler) {
        my $event = $protocol_handler->parse_request($connection, $data);
        if ($event) {
            $self->add_event_to_queue($event) or $self->log->warn("Could not enqueue event $event");
        }
    } else {
        # if we don't have a protocol handler set up yet, this should be
        # the first transmission containing an initiation string
        $self->log->debug("initiating protocol handler with data [$data]");
        my $p = Grids::Protocol->new_from_initiation_string($data, $connection, {
            id => $self->id,
            use_encryption => $self->use_encryption,
        });

        unless ($p) {
            $self->log->warn("invalid initiation string [$data]");
            $connection->transport->reset($connection);
            return;
        }

        # save protocol handler
        $connection->protocol($p);

        # save peer
        $self->network->add_to_peers(peer => $connection->peer);

        # write response
        my $proto_init_resp = $p->protocol_init_response;
        $connection->write($proto_init_resp) or $self->log->error("unable to write session init response");
    }
}

sub services {
    my ($self) = @_;

    return qw/Service1 Service2/;
}

sub check_authentication {
    my ($self, $evt) = @_;

    my $session_token = $evt->session_token or return 0;
    return $self->check_session_token($session_token);
}

sub check_session_token {
    my ($self, $token) = @_;

    # FIXME: use network
    return 1;
#    my $remote = $self->sessions->{$token};

#    return $remote;
}

sub authorized_keys {
    my ($self) = @_;

    my $pubkeys = $self->configuration->get('Node.AuthorizedKeys') || {};
    return %$pubkeys;
}

no Moose;
__PACKAGE__->meta->make_immutable;
