package Grids::Protocol::Event;

use Moose;

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

has event_name => (
    is => 'rw',
    isa => 'Str',
    required => 1,
);

has args => (
    is => 'rw',
    isa => 'Maybe[HashRef]',
    default => sub { {} },
);

# timestamp?
has time => (
    is => 'rw',
    isa => 'Int',
);
has expires => (
    is => 'rw',
    isa => 'Int',
);

has target => (
    is => 'rw',
);

has source => (
    is => 'rw',
);

has message_id => (
    is => 'rw',
    isa => 'Str',
    lazy => 1,
    builder => 'build_message_id',
    predicate => 'has_message_id',
);

has signed_message_id => (
    is => 'rw',
    isa => 'Str',
);

has was_encrypted => (
    is => 'rw',
    isa => 'Bool',
);

sub proto { croak 'deprecated' }

sub name { $_[0]->event_name }

# allow us to unset the message id so it will be regenerated with a
# new id, allowing cloning of events
sub has_message_id {
    my $self = shift;
    return $self->message_id;
}

# uuid identifying this event
sub build_message_id {
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

no Moose;
__PACKAGE__->meta->make_immutable;
