package Grids::Protocol::EventQueue;

use Moose;
use Carp;

has queue => (
    is => 'rw',
    isa => 'ArrayRef',
    default => sub { [] },
);

sub add {
    my ($self, $evt) = @_;
    push @{$self->queue}, $evt;
}

sub shift {
    my ($self) = @_;
    my $next_item;

    # find first non-expired item
    while ($next_item = CORE::shift @{$self->queue}) {
        last unless $next_item->has_expired;
    }
    
    return $next_item;
}

sub empty {
    my ($self) = @_;
    $self->queue([]);
}

no Moose;
__PACKAGE__->meta->make_immutable;
