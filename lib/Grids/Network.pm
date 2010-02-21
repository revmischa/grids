# This class represents a network of identities
package Grids::Network;

use Moose;
use Class::Autouse qw/
    Grids::Address
    Grids::Identity
    Grids::Peer
    Grids::Protocol::Event
/;

use Carp qw/croak/;

# msgId => "$peer" => acked?
has known_acks => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

# peername => \@peer_instances
has peers => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

# parent node
has node => (
    is => 'rw',
    isa => 'Grids::Node',
    required => 1,
    handles => ['network_id'],
);

# add an identity to our known peers
# opts: id, address
sub add_to_peers {
    my ($self, %opts) = @_;

    my $peer = delete $opts{peer};

    unless ($peer) {
        $peer = new Grids::Peer(%opts);
    }

    $self->peers->{$peer->name}{$peer->session_token} = $peer;
}

sub remove_from_peers {
    my ($self, $peer) = @_;

    return delete $self->peers->{$peer->name};
}

# send an event to all of our known peers
sub send_to_peers {
    my ($self, $event) = @_;

#    my $target = $event->target
#        or croak "No target defined on event passed to Network->send_to_peers";

    my @peers = $self->all_peers;
    foreach my $peer (@peers) {
        # does this peer have an ack?
        if ($self->known_acks->{"$peer"}{$event->message_id}++) {
            # we've already seen an acknowledgement for this message by this peer, skip
            next;
        }

        $self->send_to_peer($peer, $event);
    }
}

sub all_peers {
    my $self = shift;

    my @sessions = values %{$self->peers}; # list of session_token => peer
    return map { values %$_ } @sessions;
}

sub send_to_peer {
    my ($self, $peer, $event) = @_;

    # get all active sessions for this peer
    my $peer_sessions = $self->peers->{$peer->name} || {};

    # send event to all sessions for this peer
    foreach my $peer_session (values %$peer_sessions) {
        $peer_session->connection->send_event($event);
    }
}

1;
