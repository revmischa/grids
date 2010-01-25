package Grids::Protocol::Connection;

use Moose;

use Carp qw/croak/;

has 'transport' => (
    is => 'rw',
    isa => 'Grids::Transport',
    required => 1,
);

has 'channel' => (
    is => 'rw',
    required => 1,
);

has 'protocol' => (
    is => 'rw',
    isa => 'Grids::Protocol',
);

# is this the receiving end or the initiating end?
# used for determining which end of the grids protocol 
# to transmit/expect
has 'inbound' => (
    is => 'ro',
    isa => 'Bool',
    required => 1,
);

has 'peer_fingerprint_is_verified' => (
    is => 'rw',
    isa => 'Bool',
);

*is_verified = \&peer_fingerprint_is_verified;

sub outbound {
    my ($self) = @_;
    return ! $self->inbound;
}

# given an open connection and our identity,
# initialize a grids connection with another client or node
sub initiate_protocol {
    my ($self, $identity) = @_;

    croak "No identity passed to Grids::Protocol::Connection->initiate_protocol"
        unless $identity;

    my $proto = new Grids::Protocol(identity => $identity);
    my $init_string = $proto->initiation_string;

    # save protocol handler
    $self->protocol($proto);

    $self->write($init_string);
}

sub write {
    my ($self, $data) = @_;

    $self->transport->write($data, $self->channel);
}

no Moose;
__PACKAGE__->meta->make_immutable;
