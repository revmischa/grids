package Grids::Transport::Driver;

use Moose::Role;

use Carp qw/croak/;

requires qw/write connect disconnect/; # may also have: select, reset,
                                       # close_all, close_all_clients,
                                       # close_server

has 'delegate' => (
    is => 'rw',
    does => 'Grids::Transport::Consumer',
    required => 1,
    weak_ref => 1,
    handles => {
        configuration => 'configuration',
    },
);

sub reset {
    my ($self, $connection) = @_;
    return 0;
}

sub listen {
    my ($self) = @_;
    return 0;
}

sub close_all {
    my ($self) = @_;

    $self->close_all_clients;
    $self->close_server;

    return 1;
}

sub close_all_clients {
    my ($self) = @_;
    return 0;
}

sub close_server {
    my ($self) = @_;
    return 0;
}

sub select {
    my ($self) = @_;
    return 0;
}

sub delegate_do {
    my ($self, $method, @args) = @_;
    return unless $self->delegate && $self->delegate->can($method);
    return $self->delegate->$method(@args);
}

sub error {
    my ($self, $error) = @_;
    croak "Transport error: $error";
    return 0;
}

sub bind {
    my ($self, $cb, @extra) = @_;
    return sub {
        $cb->($self, @extra, @_);
    };
}

# connected established, but protocol is not yet initialized
sub connection_established {
    my ($self, $conn) = @_;

    # get identity object so we can perform cryptographic functions
    my $id = $self->delegate->id
        or die "No identity loaded in transport delegate";

    # set callbacks for crypto
    $id->set_callback('inject',     $self->bind(\&inject, $conn));
    $id->set_callback('unverified', $self->bind(\&encrypted_connection_ready, $conn, 0));
    $id->set_callback('verified',   $self->bind(\&encrypted_connection_ready, $conn, 1));
    $id->set_callback('disconnect', $self->bind(\&encrypted_connection_unready, $conn));
}

# ready to send/receive events
sub connection_ready {
    my ($self, $conn) = @_;

    $self->delegate_do('connection_ready', $conn);
}

sub inject {
    my ($self, $connection, $otr, $account, $protocol, $recipient, $message) = @_;
    
    $self->write($message, $connection);
}

#### the following methods are delegate methods, if they are defined
#### on $self->delegate they will be called


sub encrypted_connection_unready {
    my ($self, $connection, $otr, $peer_name) = @_;

    $connection->peer_fingerprint_is_verified(0);
    $self->delegate_do('encrypted_connection_unready', $connection, $peer_name);
}

sub encrypted_connection_ready {
    my ($self, $connection, $is_verified, $otr, $peer_name) = @_;

    $connection->peer_fingerprint_is_verified($is_verified);
    $self->delegate_do('encrypted_connection_ready', $connection, $peer_name);
}

sub outgoing_connection_established {
    my ($self, $connection) = @_;

    $self->connection_established($connection);
    $self->delegate_do('outgoing_connection_established', $connection);
}

sub incoming_connection_established {
    my ($self, $connection) = @_;

    $self->connection_established($connection);
    $self->delegate_do('incoming_connection_established', $connection);
}

sub data_received {
    my ($self, $connection, $data) = @_;

    $self->delegate_do('data_received', $connection, $data);
}

# called when transport has been disconnected from a peer
sub disconnected {
    my ($self, $connection) = @_;

    $self->delegate_do('disconnected', $connection);

    # TODO: unset OTR callbacks
}

1;
