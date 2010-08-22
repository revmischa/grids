# This class acts as a store for persistent client data
# TODO: actually make persistent
package Grids::Node::ClientStorage;

use Moose;
use namespace::autoclean;

use Carp qw/croak/;

# parent node
has 'node' => (
    is => 'rw',
    isa => 'Grids::Node',
    required => 1,
);

# peer id -> storage mapping
has 'store' => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

# allocate storage for a peer
sub new_storage {
    my ($self, $peer) = @_;
    return {}; # nothing fancy for now
}

# return a peer's storage
sub get_storage {
    my ($self, $peer) = @_;

    my $storage = $self->store->{$peer->id};
    unless ($storage) {
        $storage = $self->new_storage($peer);
        $self->store->{$peer->id} = $storage;
    }

    return $storage;
}

sub list {
    my ($self, $peer) = @_;

    my $storage = $self->get_storage($peer);
    my @keys = keys %{ $storage };
    return \@keys;
}

sub put {
    my ($self, $peer, $key, $value) = @_;

    my $storage = $self->get_storage($peer);
    $storage->{$key} = $value;
}

sub get {
    my ($self, $peer, $key) = @_;

    my $storage = $self->get_storage($peer);
    return $storage->{$key};
}

sub delete {
    my ($self, $peer, $key) = @_;

    my $storage = $self->get_storage($peer);
    delete $storage->{$key};
}

sub empty {
    my ($self, $peer) = @_;
    delete $self->store->{$peer->id};
}

__PACKAGE__->meta->make_immutable;
