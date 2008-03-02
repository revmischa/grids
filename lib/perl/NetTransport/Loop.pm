# This is a very simple transport that can send data to other Loop
# transports in memory

package Grids::Transport::Loop;
use strict;
use base 'Grids::Transport';

sub new {
    my ($class, $parent, %opts) = @_;

    my $self = $class->SUPER::new($parent, %opts);

    return $self;
}

sub connect {
    my ($self, $peer) = @_;
    return 0 unless $peer;

    $self->{peer} = $peer;
    $self->{peer}->{peer} = $self;

    $self->{peer}->connection_established($self);
    $self->connection_established($peer);

    return 1;
}

sub write {
    my ($self, $data, $conn) = @_;

    my $peer = $self->{peer};
    if ($peer) {
        $peer->data_received($self, $data);
    } else {
        return 0;
    }

    return 1;
}

1;
