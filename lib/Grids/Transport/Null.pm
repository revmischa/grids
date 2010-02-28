# Grids::Transport equiviliant of /dev/null

package Grids::Transport::Null;

use Moose;
with 'Grids::Transport::Driver';

has delegate => (
    is => 'rw',
);

sub write {}
sub connect {}
sub disconnect {}
sub listen {}

no Moose;
__PACKAGE__->meta->make_immutable;
