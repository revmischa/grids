use strict;
use warnings;

package Grids::UUID;

use Data::UUID;

sub new_id {
    my $uuid = new Data::UUID;
    return $uuid->create_str();
}

1;
