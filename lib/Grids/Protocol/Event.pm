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
);

sub proto { croak 'deprecated' }

sub name { $_[0]->base->{event} }

# allow us to unset the message id so it will be regenerated with a
# new id, allowing cloning of events
#clearer => 'clear_message_id',

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
