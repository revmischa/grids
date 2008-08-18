use strict;
use warnings;

package Grids::Address::TCP;

use base qw/Grids::Address/;

sub def_transport { 'Grids::Transport::TCP' }
sub port_required { 1488 }

1;
