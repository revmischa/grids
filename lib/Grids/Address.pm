package Grids::Address;

use Moose::Role;

use Carp qw/croak/;

has address => (
    is => 'rw',
    required => 1,
);

has port => (
    is => 'rw',
    isa => 'Maybe[Int]',
);

has transport => (
    is => 'rw',
    does => 'Grids::Transport',
    required => 1,
);

sub stringify {
    my $self = shift;
    return join('.', ($self->transport || 'undef'),
                $self->address,
                ($self->port || 'undef'));
}

requires qw/def_transport/;

1;
