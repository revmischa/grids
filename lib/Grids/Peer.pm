package Grids::Peer;

use Moose;
use Carp qw/croak/;
use Grids::UUID;

has connection => (
    is => 'rw',
    isa => 'Grids::Protocol::Connection',
    required => 1,
);

has session_token => (
    is => 'rw',
    isa => 'Str',
    lazy => 1,
    builder => 'build_session_token',
);

has name => (
    is => 'rw',
    isa => 'Str',
    required => 1,
);

# this should be pubkey in the future
sub id {
    my ($self) = @_;

    return $self->name;
}

sub build_session_token {
    my $self = shift;
    return Grids::UUID->new_id;
}

no Moose;
__PACKAGE__->meta->make_immutable;
