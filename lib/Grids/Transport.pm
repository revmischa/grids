package Grids::Transport;
use strict;
use warnings;
use Carp;
use IO::Select;

# use all subclasses
use Class::Autouse;
Class::Autouse->autouse_recursive('Grids::Transport');

#### Override these in subclasses

sub new {
    my ($class, $parent) = @_;

    my $self = bless { parent => $parent }, $class;
}

# write data
sub write {
    my ($self, $data, $connection) = @_;
    croak "write called on Grids::Transport base class";
}

# initiate a connection to specified address
sub connect {
    my ($self, $address) = @_;
    croak "connect called on Grids::Transport base class";
}

# optional: select filehandles and do reading/accepting/writing as appropriate
sub select {
    my ($self, $timeout) = @_;
    return 0;
}

# optional: reset to initial state
sub reset {
    my $self = shift;
    return 0;
}

##################################

# parent is whatever object owns this Transport.
# it needs to implement the "conf" method
sub parent { $_[0]->{parent} }

sub error {
    my ($self, $error) = @_;
    croak "Transport error: $error";
    return 0;
}

sub outgoing_connection_established {
    my ($self, $con) = @_;

    return unless $self->parent->can('outgoing_connection_established');
    $self->parent->outgoing_connection_established($self, $con);
}

sub incoming_connection_established {
    my ($self, $con) = @_;

    return unless $self->parent->can('incoming_connection_established');
    $self->parent->incoming_connection_established($self, $con);
}

sub data_received {
    my ($self, $connection, $data) = @_;

    return unless $self->parent->can('data_received');
    $self->parent->data_received($self, $data, $connection);
}

1;
