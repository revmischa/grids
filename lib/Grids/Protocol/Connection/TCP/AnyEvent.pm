package Grids::Protocol::Connection::TCP::AnyEvent;

use Moose;
extends 'Grids::Protocol::Connection';

has channel => (
    is => 'rw',
    isa => 'GlobRef',
    required => 1,
);

has read_watcher => (
    is => 'rw',
    required => 1,
);

no Moose;
__PACKAGE__->meta->make_immutable;
