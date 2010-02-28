# this provides functionality common to Grids::Client and Grids::Node

package Grids::Base;

use Moose::Role;
    with 'Grids::Hookable';

use Carp qw/croak/;
use Grids::Conf;

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
);

has 'transports' => (
    is => 'rw',
    isa => 'ArrayRef',
    lazy => 1,
    default => sub { [] },
);

has 'debug' => (
    is => 'rw',
    isa => 'Bool',
    default => 0,
);

has 'event_queue' => (
    is => 'rw',
    isa => 'Grids::Protocol::EventQueue',
    default => sub { Grids::Protocol::EventQueue->new; },
);

has 'transport_driver' => (
    is => 'rw',
    isa => 'Str',
    default => 'TCP::AnyEvent',
);

has 'encapsulation_class' => (
    is => 'rw',
    isa => 'Str',
    default => 'JSON',
);

has 'use_encryption' => (
    is => 'rw',
    isa => 'Bool',
    default => sub { 1 },
);

# dispatch events as they come in, don't wait for do_next_event() to
# be called.
has 'auto_flush_queue' => (
    is => 'rw',
    isa => 'Bool',
    default => sub { 0 },
);

sub _conf_builder {
    my ($self) = @_;
    return Grids::Conf->new;
}

sub new_transport {
    my ($self, $driver_class) = @_;

    $driver_class ||= $self->transport_driver;
    my $encapsulation_class = $self->encapsulation_class;

    my $proto = Grids::Protocol->new(encapsulation_class => $encapsulation_class, id => $self->id)
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

# sends an event
# FIXME: rename to something less ambiguous
sub dispatch_event {
    my ($self, $evt_name, $args) = @_;
    $self->do_request(event_name => $evt_name, event_args => $args);
}

# fetches next event from event queue and handles it
# returns true if handled an event
sub do_next_event {
    my ($self) = @_;

    my $event = $self->event_queue->shift
        or return 0;

    my $conn = $event->connection
        or croak "Invalid Event record in queue: missing connection";

    my $args = $event->args;

    # debugging
    if ($self->debug) {
        $args ||= {};

        my $args_disp = %$args ? ' (' . join(', ', map { $_ . ' = ' . $args->{$_} } keys %$args) . ')' : '';
        $self->dbg("handling event " . $event->event_name . "$args_disp");
    }

    # run hooks for this event
    my $hook_results = eval {
        $self->run_event_hooks($event);
    };

    # capture errors from hooks
    if ($@) {
        $self->warn("error while running hooks for event " . $event->event_name . ": " .
            $@ . "\n");
    }

    # were there any results?
    if ($hook_results && @$hook_results) {
        # if any hooks returned hashrefs of request arguments, do those requests
        foreach my $res (@$hook_results) {
            next unless ref $res && ref $res eq 'HASH';

            # default the return request to be of the same method
            my $res_evt = $res->{event} || $event->event_name;

            # do request
            $self->do_request(event_name => $res_evt,
                              event_args => $res,
                              connection => $conn);
        }
    }

    return 1;
}

sub enqueue_event {
    my ($self, $event_name, $connection, $args) = @_;

    croak "enqueue_event() requires a connection"
        unless $connection;

    # construct event record
    my $evt = new Grids::Protocol::Event(
        connection => $connection,
        args       => $args,
        event_name => $event_name,
    );

    return $self->add_event_to_queue($evt);
}

sub do_request {
    my ($self, %opts) = @_;

    my $connection = delete $opts{connection} or Carp::confess("No connection found in event");
    my $event = delete $opts{event_name} or Carp::confess("No event name found in event");
    my $args = delete $opts{event_args} || {};    

    my $proto = $connection->protocol;
    my $serialized_data = $proto->encapsulate($event, $args);
    return 0 unless $serialized_data;

    return $connection->write($serialized_data);
}


# attempt to connect to a Node
sub connect {
    my ($self, $address) = @_;

    my $t = $self->new_transport;
    return $t->connect($address);
}

sub dbg {
    my ($self, $msg) = @_;
    return unless $self->debug;
    my $class = $self->meta->name;
    my $name = $self->id->name || '(no identity)';
    print($name . '@' . "$class: [Debug] $msg\n");
}

sub warn {
    my ($self, $msg) = @_;
    my $class = $self->meta->name;
    my $name = $self->id->name || '(no identity)';
    print STDERR $name . '@' . "$class: [Warn] $msg\n";
}

1;

