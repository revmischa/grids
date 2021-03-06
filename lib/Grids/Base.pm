# this provides functionality common to Grids::Client and Grids::Node

package Grids::Base;

use Moose::Role;
    with 'Grids::Hookable';
    with 'Grids::Logger';

use Carp qw/croak/;
use Grids::Conf;
use Grids::Code;
use Grids::ProcessTable;

requires qw/data_received/;

has 'id' => (
    is => 'rw',
    isa => 'Grids::Identity',
    required => 1,
    handles => [qw/name/],
);

has 'configuration' => (
    is => 'rw',
    isa => 'Grids::Conf',
    lazy => 1,
    builder => '_conf_builder',
    handles => [qw/get_conf/],
);

has 'process_table' => (
    is => 'rw',
    isa => 'Grids::ProcessTable',
    lazy => 1,
    builder => '_process_table_builder',
    handles => [qw/run_program/],
);

has 'transports' => (
    is => 'rw',
    isa => 'ArrayRef',
    lazy => 1,
    default => sub { [] },
);

has 'event_queue' => (
    is => 'rw',
    isa => 'Grids::Protocol::EventQueue',
    default => sub { Grids::Protocol::EventQueue->new; },
    lazy => 1,
);

has 'transport_driver' => (
    is => 'rw',
    isa => 'Str',
    default => 'TCP::AnyEvent',
    lazy => 1,
);

has 'serialization_class' => (
    is => 'rw',
    isa => 'Str',
    default => 'ProtocolBuffer',
    lazy => 1,
);

has 'use_encryption' => (
    is => 'rw',
    isa => 'Bool',
    default => 1,
);

# dispatch events as they come in, don't wait for do_next_event() to
# be called.
has 'auto_flush_queue' => (
    is => 'rw',
    isa => 'Bool',
    lazy => 1,
    default => sub { 1 },
);

has 'autosave_configuration' => (
    is => 'rw',
    isa => 'Bool',
    lazy => 1,
    default => sub { 0 },
);

has 'autoload_configuration' => (
    is => 'rw',
    isa => 'Bool',
    lazy => 1,
    default => sub { 0 },
);

sub DEMOLISH {
    my $self = shift;

    $self->configuration->save if $self->autosave_configuration;
    $self->disconnect_all;
}

sub BUILD {
    my $self = shift;

    $self->configuration->load if $self->autoload_configuration;
}

sub _conf_builder {
    my ($self) = @_;
    return Grids::Conf->new;
}

sub _process_table_builder {
    my ($self) = @_;
    return Grids::ProcessTable->new;
}

sub new_transport {
    my ($self, $driver_class) = @_;

    $driver_class ||= $self->transport_driver;
    my $serialization_class = $self->serialization_class;

    my $proto = Grids::Protocol->new(serialization_class => $serialization_class, id => $self->id)
        or die "Failed to create protocol handler";

    my $t_class = "Grids::Transport::$driver_class";
    eval "use $t_class; 1;" or die "Could not load transport driver $driver_class: $@";

    my $t = $t_class->new(delegate => $self);

    # FIXME: need to remove this when done with it
    push @{$self->transports}, $t;

    return $t;
}

### shared methods for Client and Node

sub enable_encryption {
    my ($self) = @_;
    $self->use_encryption(1);
}

sub disable_encryption {
    my ($self) = @_;
    $self->use_encryption(0);
}

sub activate_encryption {
    my ($self) = @_;
    $self->enable_encryption;
}

sub deactivate_encryption {
    my ($self) = @_;
    $self->disable_encryption;
}

# processes everything in the event queue
sub flush_event_queue {
    my ($self) = @_;
    while ($self->do_next_event) {} 
    return 0;
}

sub add_event_to_queue {
    my ($self, $evt) = @_;

    $self->event_queue->add($evt);
    $self->flush_event_queue if $self->auto_flush_queue;
    return $evt;
}

sub listen {
    my ($self) = @_;

    my $t = $self->new_transport; 
    $t->listen;
}

sub disconnect_all_clients {
    my ($self) = @_;

    $_->close_all_clients for @{$self->transports};
}

sub disconnect_all {
    my ($self) = @_;

    $_->disconnect for @{$self->transports};
    $self->transports([]);
}

# request for Socialist Millionaire's Protocol
# (http://en.wikipedia.org/wiki/Socialist_millionaire)
sub smp_requested {
    my ($self, $connection, $peer_name, $question) = @_;

    $self->enqueue_event(
        'OTR.SMPRequest', 
        $connection,
        {
            peer_name => $peer_name,
            question => $question,
        },
    );
}

sub initiate_smp {
    my ($self, $connection, $secret, $question) = @_;
    $connection->initiate_smp($secret, $question);
}

sub continue_smp {
    my ($self, $connection, $secret) = @_;
    $connection->continue_smp($secret);
}

# encrypted connection started or changed peer verified status
sub encrypted_connection_ready {
    my ($self, $connection, $peer_name) = @_;

    $self->enqueue_event('Encrypted', $connection);
    $self->log->info("encrypted connection established with " . $connection->peer->name .
               ", verified=" . $connection->peer_fingerprint_is_verified);
}

# called when an encrypted session with a peer has ended
sub encrypted_connection_unready {
    my ($self, $connection, $peer_name) = @_;

    # FIXME: "disconnected" is kinda misleading, could still have an unencrypted session active
    $self->enqueue_event('Unencrypted', $connection);  
    $self->log->info("encrypted connection with $peer_name ended");
}

# fetches next event from event queue and handles it
# returns true if handled an event
sub do_next_event {
    my ($self) = @_;

    my $event = $self->event_queue->shift
        or return;

    my $conn = $event->connection
        or croak "Invalid Event record in queue: missing connection";

    # debugging
    if ($self->log_level > 4) {
        my $args = $event->serialize;
        my $args_disp = %$args ? ' (' . join(', ', map { $_ . ' = ' . $args->{$_} } keys %$args) . ')' : '';
        $self->log->trace("handling event " . $event->event_name . "$args_disp");
    }

    # run hooks for this event
    my $hook_results = eval {
        $self->run_event_hooks($event);
    };

    # capture errors from hooks
    if ($@) {
        $self->log->warn("Error while running hooks for event "
            . $event->event_name . ": " .  $@ . "\n");
    }

    # were there any results?
    if ($hook_results && @$hook_results) {
        # if any hooks returned hashrefs of request arguments, do those requests
        foreach my $res (@$hook_results) {
            next unless ref $res && ref $res eq 'HASH';

            # default the return request to be of the same method
            my $res_evt = delete $res->{event} || $event->event_name;
            $self->log->error("Returning res event $res_evt");

            # send reply event
            my $reply = $conn->construct_event($res_evt, $res);
            $reply->clear_id;
            $reply->set_parent_id($event->id);
            $conn->send_event($reply);
        }
    }

    return $event;
}

# add an incoming event to the queue to be processed
sub enqueue_event {
    my ($self, $event_name, $connection, $args) = @_;

    croak "enqueue_event() requires a connection"
        unless $connection;

    # construct event record
    my $evt = $connection->construct_event($event_name, $args);
    return $self->add_event_to_queue($evt);
}

sub do_request {
    my ($self, %opts) = @_;

    my $connection = delete $opts{connection} or Carp::confess("No connection found in event");
    my $event = delete $opts{event_name} or Carp::confess("No event name found in event");
    my $args = delete $opts{event_args} || {};    

    return $connection->send_event($event, $args);
}

# attempt to connect to a Node
sub connect {
    my ($self, $address) = @_;

    my $t = $self->new_transport;
    return $t->connect($address);
}

around 'format_log_message' => sub {
    my ($orig, $self, $msg) = @_;
    my $class = $self->meta->name;
    my $name = $self->id->name || '(no identity)';
    return $self->$orig($name . '@' . "$class: $msg");
};

1;

