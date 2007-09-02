# This is a very simple transport that can send data to other Loop
# transports in memory

package NetTransport::Loop;
use strict;
use base 'NetTransport';

sub new {
    my ($class, $parent, %opts) = @_;

    my $self = $class->SUPER::new($parent, %opts);

    return $self;
}

sub connect {
    my ($self, $peer) = @_;
    return 0 unless $peer;

    $self->{peer} = $peer;
    $self->connection_established($peer);

    $self->{peer}->{peer} = $self;
    $self->{peer}->connection_established($self);

    return 1;
}

sub write {
    my ($self, $data) = @_;

    my $peer = $self->{peer};
    if ($peer) {
        $peer->data_received($data);
    } else {
        return 0;
    }

    return 1;
}

1;
