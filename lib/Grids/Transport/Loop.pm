# This is a very simple transport that can send data to other Loop
# transports in memory

package Grids::Transport::Loop;

use Moose;
    with 'Grids::Transport::Driver';

use Grids::Protocol::Connection;

has 'peer_conn' => (
    is => 'rw',
    isa => 'Maybe[Grids::Protocol::Connection]',
);

has 'peer' => (
    is => 'rw',
    isa => 'Maybe[Grids::Transport::Loop]',
);

use Carp qw/croak confess/;

sub DEMOLISH {
    my ($self) = @_;
    $self->disconnect;
}

sub listen {}

sub disconnect {
    my ($self) = @_;

    return unless $self->peer && $self->peer_conn;
    $self->peer->disconnected($self->peer->peer_conn);
    $self->disconnected($self->peer_conn);

    $self->peer(undef);
    $self->peer_conn(undef);
}

sub connect {
    my ($self, $peer) = @_;

    croak __PACKAGE__ . "::connect() called without peer"
        unless $peer;

    $self->peer($peer);
    $self->peer->peer($self);
 
    my $in_conn = Grids::Protocol::Connection->new(transport => $peer, channel => $self, inbound => 1);
    $peer->peer_conn($in_conn);
    $self->peer_conn( Grids::Protocol::Connection->new(transport => $self, channel => $peer, inbound => 0) );

    $peer->incoming_connection_established($peer->peer_conn);
    $self->outgoing_connection_established($self->peer_conn);

    return $self->peer_conn;
}

sub write {
    my ($self, $data, $connection) = @_;

    confess "Attempted to write on an unconnected loop transport"
        unless $connection && $connection->channel;

    $connection->channel->data_received($self->peer->peer_conn, $data);

    return 1;
}

no Moose;
__PACKAGE__->meta->make_immutable;

