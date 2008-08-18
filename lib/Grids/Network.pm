# This class represents a network of identities

use strict;
use warnings;

package Grids::Network;

use Class::Autouse qw/
    Grids::Address
    Grids::Identity
    Grids::Peer
    Grids::Protocol::Event
/;

use Carp qw/croak/;

use base qw/Class::Accessor::Fast/;

__PACKAGE__->mk_accessors(qw/network_id known_peers known_acks/);

sub new {
    my ($class, %opts) = @_;

    my $network_id = delete $opts{network_id}
        or croak "No network_id specified when creating a network";

    my $self = {
        network_id => $network_id,
        known_acks => {}, # msgId => "$peer" => acked?
        known_peers => {}, # "$peer" => $peer
    };

    return bless $self, $class;
}

# add an identity to our known peers
# opts: id, address
sub add_to_peers {
    my ($self, %opts) = @_;

    my $peer = $opts{peer};

    unless ($peer) {
        my Grids::Identity $peer_id = $opts{id} or croak "No id specified";
        my Grids::Address $address = $opts{address} or croak "No address specified";

        $peer = new Grids::Peer(id => $peer_id, address => $address);
    }

    $self->{peers}->{$peer->stringify} = $peer;
}

sub remove_from_peers {
    my ($self, $peer) = @_;

    return delete $self->{peers}->{$peer->stringify};
}

# send an event to all of our known peers
sub send_to_peers {
    my ($self, $event) = @_;

    my $target = $event->target
        or croak "No target defined on event passed to Network->send_to_peers";

    my @peers = values %{$self->peers};

    foreach my $peer (@peers) {
        # does this peer have an ack?
        if ($self->known_acks->{"$peer"}++) {
            # we've already seen an acknowledgement for this message by this peer, skip
            next;
        }

        $self->send_to_peer($peer, $event);
    }
}

sub send_to_peer {
    my ($self, $peer, $event) = @_;

    # uhhhhhhh do this somehow
}

1;
