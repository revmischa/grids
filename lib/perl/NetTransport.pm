package NetTransport;
use strict;
use warnings;
use Carp;

# use all subclasses
use Class::Autouse;
Class::Autouse->autouse_recursive('NetTransport');

#### Override these in subclasses

sub new {
    my ($class, $parent) = @_;

    return bless { parent => $parent }, $class;
}

# write data
sub write {
    my ($self, $data) = @_;
    croak "write called on NetTransport base class";
}

# initiate a connection to specified address
sub connect {
    my ($self, $address) = @_;
    croak "connect called on NetTransport base class";
}

# optional: wait in a loop for connections
sub accept_loop {
    my $self = shift;
    return 0;
}

##################################

# parent is whatever object owns this Transport.
# it needs to implement the "conf" method
sub parent { $_[0]->{parent} }

sub connection_established {
    my ($self, $con) = @_;

    return unless $self->parent->can('connection_established');
    $self->parent->connection_established($self, $con);
}

sub data_received {
    my ($self, $data) = @_;

    return unless $self->parent->can('data_received');
    $self->parent->data_received($self, $data);
}

1;
