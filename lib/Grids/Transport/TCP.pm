package Grids::Transport::TCP;

use Moose;
    with 'Grids::Transport::Driver';

use Carp qw/croak/;

sub BUILD {
    my ($self) = @_;

    $self->configuration->add_conf(port => 1488); # add default port
}

# handled by subclasses
sub write {}
sub disconnect {}
sub connect {}
sub reset {}
sub listen {}

no Moose;
__PACKAGE__->meta->make_immutable;
