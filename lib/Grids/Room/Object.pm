# This class represents an object in a room, it holds a hash
# of user-specified attributes
package Grids::Room::Object;

use Moose;
use Grids::UUID;

has id => (
    is => 'rw',
    isa => 'Str',
    lazy => 1,
    builder => 'build_id',
);

has attr => (
    is => 'rw',
    isa => 'Maybe[HashRef]',
);

sub build_id {
    my ($self) = @_;
    return Grids::UUID->new_id;
}

no Moose;
__PACKAGE__->meta->make_immutable;
