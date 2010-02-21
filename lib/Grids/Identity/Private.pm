package Grids::Identity::Private;

use strict;
use warnings;
#use base qw/Crypt::RSA::Key::Private::SSH/;

sub new {
    my $class = shift;
    return $class->SUPER::new(KF => 'SSH', @_);
}

sub serialize {
    my $self = shift;

    my $serialized = $self->SUPER::serialize;
    return $serialized;
}

sub deserialize {
    my $class = shift;

    my $self = $class->new;
    return $self->SUPER::deserialize(@_);
}

1;
