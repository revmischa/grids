package Grids::Protocol::Connection;

use Moose;

use Carp qw/croak/;

has 'transport' => (
    is => 'rw',
    does => 'Grids::Transport::Driver',
    required => 1,
);

has 'channel' => (
    is => 'rw',
    required => 1,
);

has 'protocol' => (
    is => 'rw',
    isa => 'Grids::Protocol',
    handles => [qw/parse_request encapsulate id peer peer_name/],
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

sub DEMOLISH {
    my $self = shift;
    $self->teardown_protocol;
}

sub outbound {
    my ($self) = @_;
    return ! $self->inbound;
}

# given an open connection and our identity, initialize a grids
# connection with another client or node
sub initiate_protocol {
    my ($self, %proto_opts) = @_;

    croak "No identity passed to Grids::Protocol::Connection->initiate_protocol"
        unless $proto_opts{identity};

    my $proto = new Grids::Protocol(%proto_opts);
    my $init_string = $proto->initiation_string;

    # save protocol handler
    $self->protocol($proto);

    $self->write($init_string);
}

# done with this connection, clean up
sub teardown_protocol {
    my ($self) = @_;

    return unless $self->protocol && $self->peer;
    $self->protocol->end_encrypted_connection;
    $self->transport->reset;
}

sub write {
    my ($self, $data) = @_;

    $self->transport->write($data, $self->channel);
}

sub send_event {
    my ($self, $evt, $args) = @_;

    my $event_name;

    if (ref $evt) {
        # got passed an Event instance
        $event_name = $evt->event_name;
        $args = $evt->args;
    } else {
        $event_name = $evt;
    }

    my $msg = $self->encapsulate($event_name, $args);
    return unless $msg;
    $self->write($msg);
}

no Moose;
__PACKAGE__->meta->make_immutable;
