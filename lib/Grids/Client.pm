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

    $evt->{connection} = $connection;
    $self->event_queue->add($evt);
}

# we only have one connection (unlike Node) so we can add it
around do_request => sub {
    my ($orig, $self, %opts) = @_;

    # don't modify original args, someone might get upset
    my $argsref = $opts{event_args};
    my %args = $argsref ? %$argsref : ();

    $args{_session_token} ||= $self->session_token if $self->session_token;
    $opts{connection} ||= $self->connection if $self->connection;

    $opts{event_args} = \%args if keys %args;

    return $self->$orig(%opts);
};

# same as above
around enqueue_event => sub {
    my ($orig, $self, $event_name, $connection, $args) = @_;
    $connection ||= $self->connection;
    $self->$orig($event_name, $args, $connection);
};

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
    $self->dispatch_event('Authentication.Login');
}

no Moose;
__PACKAGE__->meta->make_immutable;
