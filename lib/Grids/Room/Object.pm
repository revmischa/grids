# This class represents an object in a room, it holds a hash
# of user-specified attributes
package Grids::Room::Object;

use strict;
use warnings;

use base qw/Class::Accessor/;
__PACKAGE__->mk_accessors(qw/attr/);

sub new {
    my ($class, $attr) = @_;

    my $self = bless {
        attr => $attr,
    }, $class;

    return $self;
}

1;
