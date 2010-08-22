package Grids::Protocol::Event;

use Moose::Role;
use namespace::autoclean;
use Carp;
use Grids::UUID;

has connection => (
    is => 'rw',
    isa => 'Grids::Protocol::Connection',
);

has transport => (
    is => 'rw',
    isa => 'Grids::Transport',
);

has was_encrypted => (
    is => 'rw',
    isa => 'Bool',
    default => 0,
);

sub proto { croak 'deprecated' }

sub name          { $_[0]->base->{event} }
sub event_name    { $_[0]->base->{event} }
sub expires       { $_[0]->base->{expires} }
sub timestamp     { $_[0]->base->{timestamp} }
sub id            { $_[0]->base->{id} }
sub signed_id     { $_[0]->base->{signed_id} }
sub source        { $_[0]->base->{source} }
sub destination   { $_[0]->base->{destination} }
sub is_success    { $_[0]->base->{is_success} }
sub error         { $_[0]->base->{error} }
sub is_broadcast  { $_[0]->base->{is_broadcast} }
sub is_ack        { $_[0]->base->{is_ack} }
sub session_token { $_[0]->base->{session_token} }

sub clear_broadcast_flag {
    my ($self) = @_;
    $self->base->{is_broadcast} = 0;
}

sub set_broadcast_flag {
    my ($self) = @_;
    $self->base->{is_broadcast} = 1;
}

sub set_session_token {
    my ($self, $tok) = @_;
    $self->base->{session_token} = $tok;
}

around 'id' => sub {
    my ($orig, $self, $new) = @_;

    if (! $self->base->{id} && ! $new) {
        $self->base->{id} = $self->build_id;
    }

    $self->base->{id} = $new if defined $new;

    return $self->$orig($new);
};

# used for event cloning (broadcasts, etc)
sub clear_id {
    my ($self) = @_;
    delete $self->base->{id};
}

# uuid identifying this event
sub build_id {
    my $self = shift;
    return Grids::UUID->new_id;
}

sub has_expired {
    my ($self) = @_;

    return 0 unless $self->expires;

    my $elapsed = time() - $self->time;
    return 1 if $elapsed > $self->expires;

    return 0;
}

# return this event as a hashref
sub serialize {
    my ($self) = @_;

    unless ($self->base->{id}) {
        $self->base->{id} = $self->build_id;
    }

    my %fields = %$self;
    return \%fields;
}

1;
