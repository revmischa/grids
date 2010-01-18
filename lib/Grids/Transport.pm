package Grids::Transport;
use strict;
use warnings;
use Carp;
use IO::Select;

use base qw/Class::Accessor::Fast/;
__PACKAGE__->mk_accessors(qw/conn/);

# use all subclasses
use Class::Autouse;
Class::Autouse->autouse_recursive('Grids::Transport');

#### Override these in subclasses

sub new {
    my ($class, $delegate) = @_;

    my $self = bless { delegate => $delegate }, $class;
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

# delegate is whatever object wants to receive events from this
# Transport.  It needs to implement the "conf" method.
sub delegate { $_[0]->{delegate} }

sub error {
    my ($self, $error) = @_;
    croak "Transport error: $error";
    return 0;
}

sub outgoing_connection_established {
    my ($self, $conn) = @_;

    $self->{conn} ||= $conn;

    return unless $self->delegate->can('outgoing_connection_established');
    $self->delegate->outgoing_connection_established($self, $conn);
}

sub incoming_connection_established {
    my ($self, $conn) = @_;

    $self->{conn} ||= $conn;

    return unless $self->delegate->can('incoming_connection_established');
    $self->delegate->incoming_connection_established($self, $conn);
}

sub data_received {
    my ($self, $connection, $data) = @_;

    return unless $self->delegate->can('data_received');
    $self->delegate->data_received($self, $data, $connection);
}

1;
