# This represents a "Room" in a 3d environment

package Grids::Room;

use strict;
use warnings;
use Data::UUID;
use base qw/Class::Accessor::Fast/;

__PACKAGE__->mk_accessors(qw/id/);

sub create {
    my ($class, %opts) = @_;

    my $uuid = new Data::UUID;
    my $id = $uuid->create_hex;

    my $self = {
        id => $id,
    };

    return bless $self, $class;
}

1
