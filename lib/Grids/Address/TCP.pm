package Grids::Address::TCP;

use Moose;
    with 'Grids::Address';

has port => (
    is => 'rw',
    isa => 'Int',
    default => sub { 1488 },
);


sub def_transport { 'Grids::Transport::TCP' }

no Moose;
__PACKAGE__->meta->make_immutable;

