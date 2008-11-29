# This class represents an object in a room, it holds a hash
# of user-specified attributes
package Grids::Room::Object;

use strict;
use warnings;

use Grids::UUID;

use base qw/Class::Accessor/;
__PACKAGE__->mk_accessors(qw/id attr/);

sub create {
    my ($class, $attr) = @_;

    my $id = Grids::UUID->new_id;

    my $self = bless {
        id => $id,
        attr => $attr,
    }, $class;

    return $self;
}

1;
