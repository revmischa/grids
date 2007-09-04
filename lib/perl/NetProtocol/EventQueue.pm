package NetProtocol::EventQueue;
use strict;
use warnings;
use Carp;
use Class::Autouse(qw/NetProtocol::Event/);

use base qw/Class::Accessor::Fast/;
__PACKAGE__->mk_accessors(qw/queue/);

sub new {
    my ($class, %opts) = @_;

    my $self = {
        queue => [],
    };

    bless $self, $class;
    return $self;
}

sub add {
    my ($self, $evt) = @_;
    push @{$self->queue}, $evt;
}

sub shift {
    my ($self) = @_;
    my $next_item;

    # find first non-expired item
    while ($next_item = CORE::shift @{$self->queue}) {
        last unless $next_item->expired;
    }
    
    return $next_item;
}

sub empty {
    my ($self) = @_;
    $self->queue([]);
}

1;
