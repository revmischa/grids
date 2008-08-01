package Grids::Protocol::Encapsulation::JSON;
use strict;
use warnings;
use Carp;
use JSON;

sub new {
    my ($class, %opts) = @_;
    my $self = {
        json => JSON->new(pretty => 0),
    };

    return bless $self, $class;
}

sub encapsulate {
    my ($self, $params) = @_;
    return $self->{json}->encode($params);
}

sub decapsulate {
    my ($self, $data) = @_;
    return $self->{json}->decode($data);
}

1;
