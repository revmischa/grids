package Grids::Transport::TCP;

use Moose;
    with 'Grids::Transport::Driver';

use Carp qw/croak/;
use IO::Select;
use IO::Socket::INET;

use Grids::Protocol::Connection;
use Grids::Address::IPv4;

has 'sockets' => (
    is => 'rw',
    isa => 'ArrayRef',
    default => sub { [] },
);

has 'connections' => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

has 'listen_sock' => (
    is => 'rw',
    isa => 'IO::Socket::INET',
);

has 'read_set' => (
    is => 'rw',
    lazy => 1,
    isa => 'IO::Select',
    default => sub { IO::Select->new },
);

sub BUILD {
    my ($self) = @_;

    $self->configuration->add_conf(port => 1488); # add default port
}

sub disconnect {
    my ($self, $connection) = @_;

    $self->remove_socket_from_connection($connection);
}

sub connect {
    my ($self, $addr) = @_;
    
    return undef unless $addr;

    croak "must pass Grids::Address to Grids::Transport::TCP->connect"
        unless $addr->does('Grids::Address');

    my $sock = IO::Socket::INET->new(Proto     => "tcp",
                                     Blocking  => 1,
                                     Reuse     => 1,
                                     PeerAddr  => $addr->address,
                                     PeerPort  => $addr->port,
                                     ) or return $self->error("Could not connect to host $addr: $!");

    my $connection = Grids::Protocol::Connection->new(transport => $self, channel => $sock, inbound => 0);
    $self->add_socket($sock, $connection);
    $self->outgoing_connection_established($connection);
}

sub write {
    my ($self, $data, $sock) = @_;

    my $datalen = length $data;
    my $datalen_packed = pack("N", $datalen);

    if ($sock && $sock->connected) {
        my $byte_count = $sock->syswrite($datalen_packed . $data);
    } else {
        Carp::confess("Tried to send data '$data' to unconnected transport");
        $self->error("Tried to send data [$data] to unconnected transport");
        return 0;
    }

    return 1;
}

sub new_listen_sock {
    my ($self) = @_;

    return $self->listen_sock(IO::Socket::INET->new(Proto     => "tcp",
                                                    Blocking  => 0,
                                                    Reuse     => 1,
                                                    LocalPort => $self->delegate->configuration->get_conf('port'),
                                                    Listen    => 16,
                              )) or die $!;
}

sub close_all {
    my ($self) = @_;
    $_->close foreach @{$self->sockets};
}

after reset => sub {
    my ($self, $connection) = @_;

    $self->remove_socket_from_connection($connection);
};

# remove the socket that is associated with this connection
sub remove_socket_from_connection {
    my ($self, $connection) = @_;

    my $socket = $connection->channel;
    return unless $socket;

    $self->remove_socket($socket);
    $socket->close;
}

sub add_socket {
    my ($self, $socket, $connection) = @_;

    $self->connections->{$socket} = $connection;

    return if grep { $_ eq $socket } @{$self->sockets};
    push @{$self->sockets}, $socket;
    $self->add_to_read_set($socket);
}

sub remove_socket {
    my ($self, $socket) = @_;

    delete $self->connections->{$socket};

    $self->sockets([ grep { $_ ne $socket } @{$self->sockets} ]);
    $self->remove_from_read_set($socket);
}

sub add_to_read_set {
    my ($self, $socket) = @_;

    $self->read_set->add($socket);
}

sub remove_from_read_set {
    my ($self, $socket) = @_;
    $self->read_set->remove($socket);
}

sub listen {
    my $self = shift;

    # create main listening socket and add it to our read_set
    my $ls = $self->new_listen_sock;
    $self->add_socket($ls);
    $ls->listen;
}

sub select {
    my ($self, $timeout) = @_;

    $timeout ||= 1;

    my ($rh_set) = IO::Select->select($self->read_set, undef, undef, $timeout);
    foreach my $rh (@$rh_set) {

        if ($rh eq $self->listen_sock) {
            # this is the main read socket, accept the connection
            # and add it to the read set
            my $ns = $rh->accept;

            my $connection = Grids::Protocol::Connection->new(transport => $self, channel => $ns, inbound => 1);
            $self->add_socket($ns, $connection);
            $self->incoming_connection_established($connection);
        } else {
            # otherwise this is a normal socket reading for reading

            my $buf = '';
            my $read = 0;

            $read = $rh->sysread($buf, 4);

            if ($read) {
                # $buf should contain a long in network byte order telling us how long the rest of 
                # this message is, followed by a comma
                my $incoming_len = unpack("N", $buf);

                if ($read && $incoming_len) {
                    $read = $rh->sysread($buf, $incoming_len);
                    if ($read != $incoming_len) { # sit and read in the rest
                        my $read_bytes = $read;
                        my $remaining_bytes = $incoming_len - $read_bytes;

                        # keep reading until we get everything
                        do {
                            my $buf_2;
                            $read = $rh->sysread($buf_2, $remaining_bytes);
                            $buf .= $buf_2;

                            $read_bytes += $read;
                            $remaining_bytes -= $read;
                        } while ($remaining_bytes);
                    } else {
                        # create/load connection instance
                        my $connection = $self->connections->{$rh} ||
                            Grids::Protocol::Connection->new(transport => $self, channel => $rh, inbound => 0);
                        $self->connections->{$rh} ||= $connection;

                        # chill, read message, process it
                        $self->data_received($connection, $buf);
                    }
                }
            } else {
                # socket is closed
                my $connection = $self->connections->{$rh};
                $self->remove_socket($rh);
                $rh->close;
            }
        }
    }

    return 1;
}

no Moose;
__PACKAGE__->meta->make_immutable;
