use strict;
package NetTransport::TCP;
use base 'NetTransport';
use IO::Socket::INET;

sub new {
    my ($class, $parent, %opts) = @_;

    my $self = $class->SUPER::new($parent, %opts);
    $self->parent->conf->add_conf(port => 1488); # add default port

    return $self;
}

sub connect {
    my ($self, $addr) = @_;
    
    return undef unless $addr;

    $self->{sock} = IO::Socket::INET->new(Proto     => "tcp",
                                          Blocking  => 1,
                                          Reuse     => 1,
                                          PeerAddr  => $addr,
                                          PeerPort  => 1488,
                                          ) or return $self->error("Could not connect to host $addr: $!");

    $self->connection_established($self->{sock});
}
sub write {
    my ($self, $data) = @_;

    my $sock = $self->{sock};
    if ($sock && $sock->connected) {
        print $sock $data;
    } else {
        $self->error("Tried to send data [$data] to unconnected transport");
        return 0;
    }

    return 1;
}

sub listen_sock {
    my ($self) = @_;

    return $self->{sock} = IO::Socket::INET->new(Proto     => "tcp",
                                                 Blocking  => 0,
                                                 Reuse     => 1,
                                                 LocalPort => $self->parent->conf->get_conf('port'),
                                                 Listen    => 16,
                                                 ) or die $!;
}

sub add_to_read_set {
    my ($self, $socket) = @_;
    $self->{read_set}->add($socket);
}

sub remove_from_read_set {
    my ($self, $socket) = @_;
    $self->{read_set}->remove($socket);
}
sub select {
    my ($self, $timeout) = @_;

    $timeout ||= 1;

    # do we have a set of filehandles to select on already created?
    unless ($self->{read_set}) {
        # instantiate read_set
        my $read_set = IO::Select->new;
        $self->{read_set} = $read_set;

        # create main listening socket and add it to our read_set
        my $ls = $self->listen_sock;
        $self->add_to_read_set($ls);
        $ls->listen;
    }

    my ($rh_set) = IO::Select->select($self->{read_set}, undef, undef, $timeout);
    foreach my $rh (@$rh_set) {
        if ($rh eq $self->{sock}) {
            # this is the main read socket, accept the connection
            # and add it to the read set
            my $ns = $rh->accept;
            $self->add_to_read_set($ns);
            $self->connection_established($ns);
        } else {
            # otherwise this is a normal socket reading for reading
            my $buf = <$rh>;

            if ($buf) {
                warn "got buf: $buf";
                # got data, process it
                $self->data_received($buf);
            } else {
                # socket is closed
                $self->remove_from_read_set($rh);
                $rh->close;
                warn "socket closed: $rh";
            }
        }
    }

    return 0;
}

1;
