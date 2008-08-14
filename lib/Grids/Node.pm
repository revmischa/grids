# This is a class for a Grids Node.  
# It handles sending/receiving data over transports and handling requests

use strict;
package Grids::Node;
use Class::Autouse qw/
    Grids::Code
    Grids::VM
    Grids::Conf
    Grids::Protocol
    Grids::Protocol::Event
    Grids::Protocol::EventQueue
/;

use Grids::Transport;
use Carp qw/croak/;

use base qw/Class::Accessor::Fast Grids::Hookable/;
__PACKAGE__->mk_accessors(qw/conf transports proto sessions debug event_queue id/);
__PACKAGE__->load_hooks;

our $default_conf = { };

sub new {
    my ($class, %opts) = @_;

    my $conf = $opts{conf};
    my $debug = $opts{debug};

    my $id = $opts{id} || $opts{identity} || croak "No identity passed to Grids::Node->new";

    $debug ||= $conf->get('debug') if $conf;

    # create default configuration if none specified
    unless ($conf) {
        $conf = Grids::Conf->new;
    }

    # instantiate event queue
    my $evt_queue = Grids::Protocol::EventQueue->new;

    my $self = {
        id          => $id,
        conf        => $conf,
        transports  => [],
        debug       => $debug,
        sessions    => {},
        event_queue => $evt_queue,
        proto       => {}, # mapping of Connection => Grids::Protocol
    };

    bless $self, $class;

    return $self;
}

sub add_transport {
    my ($self, $trans_class, %opts) = @_;
    my $trans = "Grids::Transport::$trans_class"->new($self, %opts);
    push @{$self->{transports}}, $trans;
    return $trans;
}

# someone has connected to us
sub incoming_connection_established {
    my ($self, $trans, $conn) = @_;

    $self->dbg("server transport $trans received connection: $conn\n");

    # fire 'Connected' event
#    $self->enqueue_event('Connected.Incoming', { _trans => $trans, _conn => $conn });
}

# we have connected to a server
sub outgoing_connection_established {
    my ($self, $trans, $conn) = @_;

    $self->dbg("client transport $trans connected: $conn\n");

    $self->initiate_node_protocol($trans, $conn);
    # fire 'Connected' event
#    $self->enqueue_event('Connected.Outgoing', { _trans => $trans, _conn => $conn });
}

# called when a connection to another node is established to set up a
# protocol communication layer with the other node
sub initiate_node_protocol {
    my ($self, $transport, $conn) = @_;

    croak "Got invalid Connected event" unless $transport && $conn;
    croak "Trying to initate grids protocol on a node with no identity object set" unless $self->id;

    my $proto = new Grids::Protocol(identity => $self->id);
    my $init_string = $proto->initiation_string;

    # save protocol handler
    $self->proto->{$conn} = $proto;

    $transport->write($init_string, $conn);
}

sub enqueue_event {
    my ($self, $event_name, $args) = @_;

    # construct event record
    $args->{_proto} ||= $self->proto->{$args->{_conn}};
    my $evt = new Grids::Protocol::Event(
                                         params => $args, 
                                         event_name => $event_name,
                                         );

    return $self->event_queue->add($evt);
}

sub data_received {
    my ($self, $trans, $data, $connection) = @_;

    $self->dbg("received data [$data]");

    # todo: make sure this is destroyed when connection is closed
    my $protocol_handler = $self->proto->{$connection};

    if ($protocol_handler) {
        my $event = $protocol_handler->parse_request($data);
        if ($event) {
            $event->args->{_trans} = $trans;
            $event->args->{_proto} = $protocol_handler;
            $event->args->{_conn} = $connection;

            $self->event_queue->add($event) or $self->warn("Could not enqueue event $event");
        }
    } else {
        # if we don't have a protocol handler set up yet, this should be
        # the first transmission containing an initiation string
        $self->dbg("initating protocol handler");
        my $p = Grids::Protocol->new_from_initiation_string($data, identity => $self->id);

        unless ($p) {
            $self->warn("invalid initiation string [$data]");
            $trans->reset;
            return;
        }

        $self->proto->{$connection} = $p;

        my $proto_init_resp = $p->protocol_init_response;
        $trans->write($proto_init_resp, $connection) or $self->dbg("Unable to write session init response");
        $self->session_initiated($trans);
    }
}

sub session_initiated {
    my ($self, $trans) = @_;

    $self->enqueue_event('Connected', { _trans => $trans, _conn => $trans->conn });
    $self->dbg("initiated session");
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

    my $trans = delete $evt->args->{_trans}
        or die "Invalid Event record in queue: missing transport";

    my $proto = delete $evt->args->{_proto}
        or die "Invalid Event record in queue: missing protocol";

    my $conn = delete $evt->args->{_conn};

    my $hook_results = $self->run_event_hooks({
        event => $evt->event_name,
        args => $evt->args,
        trans => $trans,
        proto => $proto,
    });

    # were there any results?
    if ($hook_results && @$hook_results) {
        # if any hooks returned hashrefs of request arguments, do those requests
        foreach my $res (@$hook_results) {
            next unless ref $res && ref $res eq 'HASH';

            # default the return request to be of the same method
            my $res_evt = $res->{event} || $evt->event_name;

            # do request
            $self->do_request(transport => $trans,
                              event_name => $evt->event_name,
                              event_args => $res,
                              connection => $conn,
                              protocol_handler => $proto);
        }
    }

    return 1;
}

sub do_request {
    my ($self, %opts) = @_;

    my $trans = delete $opts{transport} or croak "No transport";
    my $proto = delete $opts{protocol_handler} or croak "No protocol handler";
    my $connection = delete $opts{connection} or croak "No connection";
    my $event = delete $opts{event_name} or croak "No event name";
    my $params = delete $opts{event_args} || {};    

    my $serialized_data = $proto->encapsulate($event, $params);
    return 0 unless $serialized_data;

    return $trans->write($serialized_data, $connection);
}

sub services {
    my ($self) = @_;

    return qw/Service1 Service2/;
}

sub check_authentication {
    my ($self, $req) = @_;

    my $session_token = delete $req->{args}{_session_token} or return 0;
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
    warn "Grids::Node:   [Debug] $msg\n";
}

sub warn {
    my ($self, $msg) = @_;
    warn "Grids::Node:   [Warn] $msg\n";
}

1;
