package Grids::Client;

use Moose;
    with 'Grids::Base';
    with 'Grids::Transport::Consumer';  # receives transport-related callbacks

use Grids::Transport;
use Grids::Protocol;
use Grids::Protocol::EventQueue;

use Carp qw (croak);

has 'connection' => (
    is => 'rw',
    isa => 'Grids::Protocol::Connection',
    handles => {
        write => 'write',
    },
);

has 'session_token' => (
    is => 'rw',
    isa => 'Str',
);

# this needs to be moosified
has 'hooks' => (
    is => 'rw',
    isa => 'HashRef',
);

use base qw/Grids::Hookable/;
__PACKAGE__->load_hooks;

# called when our transport receives data
sub data_received {
    my ($self, $connection, $data) = @_;

    $self->dbg("received data [$data]");

    my $evt = $connection->parse_request($data);
    return unless $evt;

    $evt->{_connection} = $connection;
    $self->event_queue->add($evt);
}

# processes everything in the event queue
sub flush_event_queue {
    my ($self) = @_;
    while ($self->do_next_event) {}
}

# fetches next event from event queue and handles it
# returns true if handled an event
sub do_next_event {
    my ($self) = @_;

    my $event = $self->event_queue->shift
        or return 0;

    my $args = $event->args;
    my $args_disp = %$args ? ' (' . join(', ', map { $_ . ' = ' . $args->{$_} } keys %$args) . ')' : '';
    $self->dbg("Handling event " . $event->event_name . "$args_disp");

    my @hook_results = $self->run_event_hooks($event);

    # were there any results?
    if (@hook_results) {
        # if any hooks returned hashrefs of request arguments, do those requests
        foreach my $res (@hook_results) {
            next unless ref $res && ref $res eq 'HASH';

            # default the return request to be of the same method
            my $res_evt = $res->{event} || $event->event_name;
            $self->do_request($event->event_name, $res);
        }
    }

    return 1;
}

# transmits a protocol request
sub do_request {
    my ($self, $event_name, $argsref) = @_;

    # copy args so we don't modify anything
    my %args = $argsref ? %$argsref : ();

    if ($self->session_token) {
        $args{_session_token} = $self->session_token;
    }

    $self->connection->send_event($event_name, \%args);
}

# attempt to connect to a Node
sub connect {
    my ($self, $address) = @_;

    my $transport_class = $self->transport_class;
    my $encapsulation_class = $self->encapsulation_class;

    my $proto = Grids::Protocol->new(encapsulation => $encapsulation_class, identity => $self->id)
        or die "Failed to create protocol handler";

    my $t = "Grids::Transport::$transport_class"->new(delegate => $self);

    $t->connect($address);
}

# Called when a connection with a Node has been established. This
# simply means there is a connection, but the protocol handler has not
# been set up yet. Once the connection is set up properly, the
# Connected event will be called
sub outgoing_connection_established {
    my ($self, $connection) = @_;

    $self->dbg("Connection to node successful. Initiating Grids protocol...");
    $self->connection($connection);
    $connection->initiate_protocol($self->id);
}

# initiates a login, call after ProtocolEstablished event
sub login {
    my $self = shift;
    $self->do_request('Authentication.Login');
}

sub dbg {
    my ($self, $msg) = @_;
    return unless $self->debug;
    warn "Grids::Client: [Debug] $msg\n";
}

sub warn {
    my ($self, $msg) = @_;
    warn "Grids::Client: [Warn] $msg\n";
}

no Moose;
__PACKAGE__->meta->make_immutable;
