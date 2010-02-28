package Grids::Protocol::Connection::TCP::AnyEvent;

use Moose;
extends 'Grids::Protocol::Connection';

has channel => (
    is => 'rw',
    isa => 'GlobRef',
    required => 1,
);

has client => (
    is => 'rw',
);

has server => (
    is => 'rw',
);

has handle => (
    is => 'rw',
    clearer => 'clear_handle',
);

no Moose;
__PACKAGE__->meta->make_immutable;
