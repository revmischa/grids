# This represents a "Room" in a 3d environment

package Grids::Room;

use strict;
use warnings;
use Grids::UUID;
use Grids::Room::Object;

use base qw/Class::Accessor::Fast/;
__PACKAGE__->mk_accessors(qw/id objects people/);

sub create {
    my ($class, %opts) = @_;

    my $id = Grids::UUID->new_id;

    my $self = {
        id => $id,
        objects => {},
        people => {},
    };

    return bless $self, $class;
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



1
