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
    handles => [qw/id peer peer_name has_peer/],
    predicate => 'has_protocol',
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
        unless $proto_opts{id};

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
}

sub write {
    my ($self, $data) = @_;

    return $self->transport->write($data, $self);
}

sub initiate_smp {
    my ($self, $secret, $question) = @_;
    $self->id->initiate_smp($self->peer_name, $secret, $question);
}

sub continue_smp {
    my ($self, $secret) = @_;
    $self->id->continue_smp($self->peer_name, $secret);
}

sub send_event {
    my ($self, $evt, $args) = @_;

    my $msg = $self->serialize_event($evt, $args);
    unless ($msg) {
        warn "Failed to serialize event: $evt";
        return;
    }

    return unless $msg;
    return $self->write($msg);
}

sub serialize_event {
    my ($self, $event_name, $args) = @_;

    my $evt = $self->construct_event($event_name, $args);
    my $ser = $self->protocol->serialize_event($evt);
    return $ser;
}

sub construct_event {
    my ($self, $event_name, $args) = @_;

    my $evt = $self->protocol->construct_event($event_name, $args) or return;
    $evt->connection($self);
    return $evt;
}

# opposite of serialize_event
sub parse_request {
    my ($self, $data) = @_;

    my $evt = $self->protocol->parse_request($self, $data) or return;
    $evt->connection($self);
    return $evt;
}

no Moose;
__PACKAGE__->meta->make_immutable;
