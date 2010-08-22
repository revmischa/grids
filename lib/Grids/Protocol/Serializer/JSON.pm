package Grids::Protocol::Serializer::JSON;

use Moose;
    with 'Grids::Protocol::Serializer';

use Carp;
use JSON::XS;

# parser
has json => (
    is => 'rw',
    isa => 'JSON::XS',
    lazy => 1,
    builder => 'build_parser',
);

sub build_parser {
    my ($self) = @_;
    return JSON::XS->new->pretty(0),
}

sub serialize {
    my ($self, $params) = @_;
    
    eval { $self->json->encode($params) } or Carp::confess();

    return $self->json->encode($params);
}

sub deserialize {
    my ($self, $data) = @_;
    return $self->json->decode($data);
}

no Moose;
__PACKAGE__->meta->make_immutable;
