# this provides functionality common to Grids::Client and Grids::Node

package Grids::Base;

use Moose::Role;

use Grids::Conf;

has 'id' => (
    is => 'rw',
    isa => 'Grids::Identity',
    required => 1,
);

has 'configuration' => (
    is => 'rw',
    isa => 'Grids::Conf',
    lazy => 1,
    builder => '_conf_builder',
);

has 'debug' => (
    is => 'rw',
    isa => 'Bool',
    default => 0,
);

has 'event_queue' => (
    is => 'rw',
    isa => 'Grids::Protocol::EventQueue',
    default => sub { Grids::Protocol::EventQueue->new; },
);

has 'transport_class' => (
    is => 'rw',
    default => 'TCP',
);

has 'encapsulation_class' => (
    is => 'rw',
    default => 'JSON',
);

sub _conf_builder {
    my ($self) = @_;
    return Grids::Conf->new;
}


1;

