package Grids::Protocol::Serializer::JSON;

use Moose;
    with 'Grids::Protocol::Serializer';

use namespace::autoclean;
use Grids::Protocol::Event;
use Carp qw/croak confess/;
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
    my ($self, $evt) = @_;

    my $event_name = $evt->name;
    my $obj = $evt->serialize;
    my $ret = eval { $self->json->encode($obj) }
        or confess $@;

    return $ret;
}

sub deserialize {
    my ($self, $data) = @_;

    my $obj = eval { $self->json->decode($data); }
        or warn "Failed parsing message '$data': $@";

    my $event_name = $obj->{base}{event};
    unless ($event_name) {
        warn "Failed parsing message: '$data': no event name found";
        return;
    }

    my $message_class = eval { $self->get_message_class($event_name) };
    unless ($message_class) {
        warn "Failed parsing message: '$data': $@";
        return;
    }

    my $evt = eval { $message_class->new($obj) };
    unless ($evt) {
        warn "Failed parsing message: '$data': $@";
        return;
    }

    # we parsed the message successfully, apply Event role
    Grids::Protocol::Event->meta->apply($evt);

    return $evt;
}

no Moose;
__PACKAGE__->meta->make_immutable;
