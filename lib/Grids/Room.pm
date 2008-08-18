# This represents a "Room" in a 3d environment

package Grids::Room;

use strict;
use warnings;
use Grids::UUID;
use base qw/Class::Accessor::Fast/;

__PACKAGE__->mk_accessors(qw/id/);

sub create {
    my ($class, %opts) = @_;

    my $id = Grids::UUID->new_id;

    my $self = {
        id => $id,
    };

    return bless $self, $class;
}

1
