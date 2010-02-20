package Grids::Peer;

use Moose;
use Carp qw/croak/;

# required?
has address => (
    is => 'rw',
    isa => 'Grids::Address',
);

# required?
has id => (
    is => 'rw',
    isa => 'Maybe[Grids::Identity]',
);

has name => (
    is => 'rw',
    isa => 'Maybe[Str]',
);

has session_token => (
    is => 'rw',
    isa => 'Str',
);

sub stringify {
    my $self = shift;
    return join(',', ($self->address || 'undef'), ($self->id || 'undef'));
}

1;
