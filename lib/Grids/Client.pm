package Grids::Client;

use Moose;
    with 'Grids::Base';
    with 'Grids::Transport::Consumer';  # receives transport-related callbacks

use Grids::Protocol;
use Grids::Protocol::EventQueue;
use Grids::Protocol::Event;

use Carp qw (croak);

has 'connection' => (
    is => 'rw',
    isa => 'Grids::Protocol::Connection',
    clearer => 'clear_connection',
    handles => {
        write => 'write',
    },
);

# keep reference to client connection object as long as we need it
has client => (
    is => 'rw',
    clearer => 'clear_client',
);

has 'session_token' => (
    is => 'rw',
    isa => 'Str',
);

has 'hooks' => (
    is => 'rw',
    isa => 'HashRef',
);

__PACKAGE__->load_hooks;

# called when our transport receives data
sub data_received {
    my ($self, $connection, $data) = @_;

    my $evt = $connection->parse_request($connection, $data);
    return unless $evt;

    $evt->{connection} = $connection;
    $self->add_event_to_queue($evt);
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

around connect => sub {
    my ($orig, $self, $address) = @_;

    # we can only have one connection at a time, so make sure the
    # transport is reset and all outgoing connections are closed
    # before we connect
    $self->disconnect;

    my $client = $self->$orig($address);
    $self->client($client);

    return $client;
};

sub disconnect {
    my ($self) = @_;
    
    $_->close_all_clients foreach @{$self->transports};

    $self->clear_connection;
    $self->clear_client;
}

# connection started, can send/receive events
sub connection_ready {
    my ($self, $connection) = @_;
    
    $self->dbg(($connection->inbound ? 'inbound' : 'outbound') . " connection ready with " . $connection->peer->name);
}

# encrypted connection started
sub encrypted_connection_ready {
    my ($self, $connection) = @_;
    
    $self->dbg("encrypted connection established with " . $connection->peer->name);
}

# Called when a connection with a Node has been established. This
# simply means there is a connection, but the protocol handler has not
# been set up yet. Once the connection is set up properly, the
# Connected event will be called
sub outgoing_connection_established {
    my ($self, $connection) = @_;

    $self->dbg("connection to node successful. Initiating Grids protocol...");
    $self->connection($connection);
    $connection->initiate_protocol(id => $self->id, use_encryption => $self->use_encryption);
}

# initiates a login, call after ProtocolEstablished event
sub login {
    my $self = shift;
    $self->dispatch_event('Authentication.Login');
}

sub select {
    my $self = shift;
    return 0 unless $self->connection && $self->connection->transport;
    return $self->connection->transport->select;
}

no Moose;
__PACKAGE__->meta->make_immutable;
