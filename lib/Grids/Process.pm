# This represents a program running in a VM
package Grids::Process;

use Moose;
use namespace::autoclean;

has 'vm' => (
    is => 'rw',
    isa => 'Grids::VM',
    required => 1,
    handles => [qw/run owner/],
);

has 'pid' => (
    is => 'rw',
    isa => 'Int',
    required => 1,
);

__PACKAGE__->meta->make_immutable;
