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

# actually write data
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
# Transport.  It needs to implement the "conf" and "id" accessors.
sub delegate { $_[0]->{delegate} }

sub error {
    my ($self, $error) = @_;
    croak "Transport error: $error";
    return 0;
}

sub bind {
    my ($self, $cb) = @_;
    return sub {
        $cb->($self, @_);
    };
}

sub inject {
    my ($self, $otr, $account, $protocol, $recipient, $message) = @_;
    
    $self->write($message, $self->conn);
}

sub connection_established {
    my ($self, $conn) = @_;

    $self->{conn} ||= $conn;

    # get identity object so we can perform cryptographic functions
    my $id = $self->delegate->id
        or die "No identity loaded in transport delegate";

    # set callbacks for crypto
    $id->set_callback('inject', $self->bind(\&inject));
}

sub outgoing_connection_established {
    my ($self, $conn) = @_;

    $self->connection_established($conn);

    return unless $self->delegate->can('outgoing_connection_established');
    $self->delegate->outgoing_connection_established($self, $conn);
}

sub incoming_connection_established {
    my ($self, $conn) = @_;

    $self->connection_established($conn);

    return unless $self->delegate->can('incoming_connection_established');
    $self->delegate->incoming_connection_established($self, $conn);
}

sub data_received {
    my ($self, $connection, $data) = @_;

    return unless $self->delegate->can('data_received');
    $self->delegate->data_received($self, $data, $connection);
}

1;
