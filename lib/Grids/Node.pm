# This is a class for a Grids Node.  
# It handles sending/receiving data over transports and handling requests

use strict;
use warnings;

package Grids::Node;
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

use base qw/Class::Accessor::Fast Grids::Hookable/;
__PACKAGE__->mk_accessors(qw/conf peer_connections transports sessions debug event_queue id/);
__PACKAGE__->load_hooks;

our $default_conf = { };

sub new {
    my ($class, %opts) = @_;

    my $conf = $opts{conf};
    my $debug = $opts{debug};
    my $id = $opts{id} || $opts{identity} || croak "No identity passed to Grids::Node->new";
    my $network_id = $opts{network_id} || $id; # we are our own network if no network specified
    my $network = $opts{network} || new Grids::Network(network_id => $network_id);

    $debug ||= $conf->get('debug') if $conf;

    # create default configuration if none specified
    unless ($conf) {
        $conf = Grids::Conf->new;
    }

    # instantiate event queue
    my $evt_queue = Grids::Protocol::EventQueue->new;

    my $self = {
        id          => $id,
        network     => $network,
        conf        => $conf,
        transports  => [],
        peer_connections => {}, # map of $peer->id => [$connections]
        debug       => $debug,
        sessions    => {},
        event_queue => $evt_queue,
    };

    bless $self, $class;
    return $self;
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

    push @{$self->{transports}}, $trans;
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

    $self->enqueue_event('Connected', { _connection => $connection });
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

    $self->enqueue_event('Disconnected', { _connection => $connection });
    $self->dbg("encrypted connection with $peer_name ended");
}

# called when a connection to another node is established to set up a
# protocol communication layer with the other node
sub initiate_node_protocol {
    my ($self, $connection) = @_;

    croak "Trying to initiate grids protocol on a node with no identity object set" unless $self->id;

    $connection->initiate_protocol($self->id);
}

sub enqueue_event {
    my ($self, $event_name, $args) = @_;

    # construct event record
    my $evt = new Grids::Protocol::Event(
                                         args => $args,
                                         event_name => $event_name,
                                         );

    return $self->event_queue->add($evt);
}

sub data_received {
    my ($self, $connection, $data) = @_;

    # todo: make sure this is destroyed when connection is closed
    my $protocol_handler = $connection->protocol;

    if ($protocol_handler) {
        my $event = $protocol_handler->parse_request($data);
        if ($event) {
            $event->args->{_connection}  = $connection;
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

# processes everything in the event queue
sub flush_event_queue {
    my ($self) = @_;
    while ($self->do_next_event) {} 
    return 0;
}

sub do_next_event {
    my ($self) = @_;

    my $evt = $self->event_queue->shift
        or return 0;

    my $conn = delete $evt->args->{_connection}
        or die "Invalid Event record in queue: missing connection";

    $self->dbg("Got event " . $evt->event_name);

    my $hook_results = eval {
        $self->run_event_hooks($evt);
    };

    if ($@) {
        warn "Error while running hooks for event " . $evt->event_name . ": " .
            $@ . "\n";

        return 0;
    }

    # were there any results?
    if ($hook_results && @$hook_results) {
        # if any hooks returned hashrefs of request arguments, do those requests
        foreach my $res (@$hook_results) {
            next unless ref $res && ref $res eq 'HASH';

            # default the return request to be of the same method
            my $res_evt = $res->{event} || $evt->event_name;

            # do request
            $self->do_request(event_name => $evt->event_name,
                              event_args => $res,
                              connection => $conn);
        }
    }

    return 1;
}

sub do_request {
    my ($self, %opts) = @_;

    my $connection = delete $opts{connection} or croak "No connection";
    my $event = delete $opts{event_name} or croak "No event name";
    my $params = delete $opts{event_args} || {};    

    my $proto = $connection->protocol;
    my $serialized_data = $proto->encapsulate($event, $params);
    return 0 unless $serialized_data;

    return $connection->write($serialized_data);
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

    my $pubkeys = $self->conf->get('Node.AuthorizedKeys') || {};
    return %$pubkeys;
}

sub dbg {
    my ($self, $msg) = @_;
    return unless $self->debug;
    my $name = $self->id->name;
    warn "Grids::Node:   [$name Debug] $msg\n";
}

sub warn {
    my ($self, $msg) = @_;
    my $name = $self->id->name;
    warn "Grids::Node:   [$name Warn] $msg\n";
}

1;
