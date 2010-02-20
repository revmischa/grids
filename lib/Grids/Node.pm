# This is a class for a Grids Node.  
# It handles sending/receiving data over transports and handling requests

package Grids::Node;

use Moose;
    with 'Grids::Base';
    with 'Grids::Transport::Consumer';  # receives transport-related callbacks

# moosify
use Class::Autouse qw/
    Grids::Code
    Grids::VM
    Grids::Conf
    Grids::Protocol
    Grids::Protocol::Event
    Grids::Protocol::EventQueue
    Grids::Network
/;

use Grids::Transport;
use Carp qw/croak/;

# moosify
use base qw/Grids::Hookable/;
__PACKAGE__->load_hooks;

has 'peer_connections' => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

has 'sessions' => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

has 'transports' => (
    is => 'rw',
    isa => 'ArrayRef',
    default => sub { [] },
);

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
);

# create new network instance
sub _network_builder {
    my ($self) = @_;
    return new Grids::Network($self->network_id);
}


# send an event to all nodes in this network
sub network_broadcast {
    my ($self, $event) = @_;

    my $network = $self->network;
    $network->send_to_all($event);
}

sub add_transport {
    my ($self, $trans_class, %opts) = @_;

    my $trans = "Grids::Transport::$trans_class"->new(delegate => $self, %opts);

    push @{$self->transports}, $trans;
    return $trans;
}

# someone has connected to us
sub incoming_connection_established {
    my ($self, $connection) = @_;
    $self->dbg("inbound connection established on transport " . $connection->transport);
}

# we have connected to a server
sub outgoing_connection_established {
    my ($self, $connection) = @_;
    $self->dbg("outbound connection established on transport " . $connection->transport);
    $self->initiate_node_protocol($connection);
}

# called when a protocol handler has been established and a connection
# has been made and encrypted
sub connection_ready {
    my ($self, $connection, $peer_name) = @_;

    $self->peer_connections->{$peer_name} ||= [];
    push @{$self->peer_connections->{$peer_name}}, $connection;

    $self->enqueue_event('Connected', $connection);
    $self->dbg("encrypted connection with $peer_name ready");
}

# called when an encrypted session with a peer has ended
sub connection_unready {
    my ($self, $connection, $peer_name) = @_;

    if (! $self->peer_connections->{$peer_name}) {
        warn "got connection_ready($peer_name) but no connection was established";
        return;
    }

    $self->peer_connections->{$peer_name} = [ grep { $_ != $connection } @{$self->peer_connections} ];

    $self->enqueue_event('Disconnected', $connection);
    $self->dbg("encrypted connection with $peer_name ended");
}

# called when a connection to another node is established to set up a
# protocol communication layer with the other node
sub initiate_node_protocol {
    my ($self, $connection) = @_;

    croak "Trying to initiate grids protocol on a node with no identity object set" unless $self->id;

    $connection->initiate_protocol($self->id);
}

sub data_received {
    my ($self, $connection, $data) = @_;

    # todo: make sure this is destroyed when connection is closed
    my $protocol_handler = $connection->protocol;

    if ($protocol_handler) {
        my $event = $protocol_handler->parse_request($data);
        if ($event) {
            $event->connection($connection);  # FIXME
            $self->event_queue->add($event) or $self->warn("Could not enqueue event $event");
        }
    } else {
        # if we don't have a protocol handler set up yet, this should be
        # the first transmission containing an initiation string
        $self->dbg("initiating protocol handler with data [$data]");
        my $p = Grids::Protocol->new_from_initiation_string($data, identity => $self->id);

        unless ($p) {
            $self->warn("invalid initiation string [$data]");
            $connection->transport->reset;
            return;
        }

        # save protocol handler
        $connection->protocol($p);

        my $proto_init_resp = $p->protocol_init_response;
        $connection->write($proto_init_resp) or $self->dbg("Unable to write session init response");
    }
}

sub services {
    my ($self) = @_;

    return qw/Service1 Service2/;
}

sub check_authentication {
    my ($self, $evt) = @_;

    my $session_token = $evt->{args}->{_session_token} or return 0;
    return $self->check_session_token($session_token);
}

sub check_session_token {
    my ($self, $token) = @_;

    my $remote = $self->sessions->{$token};

    return $remote;
}

sub authorized_keys {
    my ($self) = @_;

    my $pubkeys = $self->configuration->get('Node.AuthorizedKeys') || {};
    return %$pubkeys;
}

no Moose;
__PACKAGE__->meta->make_immutable;
