# This represents a "Room" in a 3d environment

package Grids::Room;

use Moose;
use Grids::UUID;
use Grids::Room::Object;

has id => (
    is => 'rw',
    isa => 'Str',
    lazy => 1,
    builder => 'build_id',
);

has objects => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

has people => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

sub build_id {
    my ($self) = @_;
    return Grids::UUID->new_id;
}

sub create_object {
    my ($self, $attr) = @_;

    my $obj = Grids::Room::Object->create($attr);
    $self->add_object($obj);

    return $obj;
}

sub add_object {
    my ($self, $obj) = @_;
    $self->objects->{$obj->id} = $obj;
}

no Moose;
__PACKAGE__->meta->make_immutable;
