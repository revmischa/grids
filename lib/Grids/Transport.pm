package Grids::Transport;

use Moose;

# moosify
use Grids::Transport::TCP;
use Grids::Transport::Loop;

no Moose;
__PACKAGE__->meta->make_immutable;
