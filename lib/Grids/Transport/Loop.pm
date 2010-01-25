# This is a very simple transport that can send data to other Loop
# transports in memory

package Grids::Transport::Loop;

use Moose;
    with 'Grids::Transport::Driver';

use Grids::Protocol::Connection;

has 'peer_conn' => (
    is => 'rw',
    isa => 'Grids::Protocol::Connection',
);

has 'peer' => (
    is => 'rw',
    isa => 'Grids::Transport::Loop',
);

use Carp qw/croak confess/;

sub received_connection {
    my ($self, $peer) = @_;

    my $in_conn = Grids::Protocol::Connection->new(transport => $self, channel => $peer, inbound => 1);

    $self->peer_conn($in_conn);
}

sub connect {
    my ($self, $peer) = @_;

    croak __PACKAGE__ . "::connect() called without peer"
        unless $peer;

    $self->peer($peer);
    $self->peer->peer($self);
 
    $peer->received_connection($self);

    $self->peer_conn( Grids::Protocol::Connection->new(transport => $self, channel => $peer, inbound => 0) );

    $peer->incoming_connection_established($peer->peer_conn);
    $self->outgoing_connection_established($self->peer_conn);

    return 1;
}

sub write {
    my ($self, $data) = @_;

    my $conn = $self->peer->peer_conn;

    confess "Attempted to write on an unconnected loop transport"
        unless $conn && $self->peer;

    $self->peer->data_received($conn, $data);

    return 1;
}

no Moose;
__PACKAGE__->meta->make_immutable;

