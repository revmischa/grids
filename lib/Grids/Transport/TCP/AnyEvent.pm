package Grids::Transport::TCP::AnyEvent;

use Moose;
    extends 'Grids::Transport::TCP';
    with 'Grids::Transport::Driver';

use Carp qw/croak/;
use AnyEvent;
use AnyEvent::Socket;

use Grids::Protocol::Connection::TCP::AnyEvent;
use Grids::Address::IPv4;

has 'listen_server' => (
    is => 'rw',
#    isa => 'Maybe[AnyEvent::Socket]',
);

has 'client_connections' => (
    is => 'rw',
    lazy => 1,
    isa => 'ArrayRef',
    default => sub { [] },
);

sub disconnect {
    my ($self, $connection) = @_;

    $self->remove_socket_from_connection($connection);
}

sub connect {
    my ($self, $addr) = @_;
    
    return undef unless $addr;

    croak "must pass Grids::Address to Grids::Transport::TCP->connect"
        unless $addr->does('Grids::Address');

    my $address = $addr->address;
    my $port = $addr->port;

    #$self->outgoing_connection_established($connection);
}

sub write {
    my ($self, $data, $connection) = @_;

    my $channel = $connection->channel
        or croak "Tried to write on a connection without a channel";

    return syswrite $channel, $data;
}

sub close_all {
    my ($self) = @_;
    
    # delete references to connections
    $self->client_connections([]);

    # stop server
    $self->listen_server(undef);
}

after reset => sub {
    my ($self, $connection) = @_;

    $self->remove_socket_from_connection($connection);
};

# given a connection, delete it from the list of client sockets
sub remove_socket_from_connection {
    my ($self, $connection) = @_;

    my $socket = $connection->channel or return;
    my $client_connections = $self->client_connections;
    my @new_conns = grep { $_ != $socket } @$client_connections;
    $self->client_connections(\@new_conns);

    # return how many sockets were removed
    return @new_conns - @$client_connections;
}

sub listen {
    my ($self) = @_;

    # create main listening socket
    $self->new_listen_sock;
}

sub new_listen_sock {
    my ($self) = @_;

    my $port = $self->delegate->configuration->get_conf('port')
        or die "Tried to listen on socket but no port defined";

    my $server = tcp_server undef, $port, sub {
        # called when client connection is accepted
        my ($fh, $host, $port) = @_;

        warn "connection received from $host:$port";

        # create a condvar to receive read events
        my $res_cv = AnyEvent->condvar;

        # create a read watcher
        my $read_watcher = AnyEvent->io(
            fh   => $fh,
            poll => "r",
            cb   => sub {
                # read in one 32-bit longword (message length)
                my $read = 0;
                my $buf;
                $read = sysread($fh, $buf, 4);

                if ($read && $read == 4 && length $buf == 4) {
                    # $buf should contain a long in network byte order telling us how long the rest of 
                    # this message is, followed by a comma
                    my $incoming_len = unpack("N", $buf);

                    # maybe we should only read a small chunk to start with, rather than the whole thing? unsure
                    my $msg_read_bytes = sysread($fh, $buf, $incoming_len);

                    if ($msg_read_bytes != $incoming_len) {
                        # didn't get the whole message in one read
                        # read in the rest of the data
                        # is it ok to block here and read? not sure

                        my $read_bytes = $msg_read_bytes; # how many bytes we have read
                        my $remaining_bytes = $incoming_len - $read_bytes; # how many more bytes we want to read
                    
                        # keep reading until we get everything
                        do {
                            my $buf_2;
                            my $chunk_size = sysread($fh, $buf_2, $remaining_bytes);
                            $buf .= $buf_2;

                            $msg_read_bytes += $chunk_size;
                            $remaining_bytes -= $chunk_size;
                        } while ($remaining_bytes);
                    }

                    if ($msg_read_bytes && $buf) {
                        # got a message, hooray
                        warn "received data [$buf]";
                        $res_cv->send($buf);
                    }
                } elsif ($read <= 0) {
                    # nothing to read, or error
                    # TODO: handle this!
                    warn "read returned: $read";
                }
            },
        );

        # create connection instance
        my $conn = Grids::Protocol::Connection::TCP::AnyEvent->new(
            transport => $self,
            channel => $fh,
            read_watcher => $read_watcher,
            inbound => 1,
        );

        # keep track of watcher
        push @{$self->client_connections}, $read_watcher;

        $res_cv->cb(sub {
            my $data = $res_cv->recv;  # non-blocking read
            $self->data_received($conn, $data);
        });

        # send connected event
        $self->incoming_connection_established($conn);
    };

    # keep track of the server until we don't want this socket around anymore
    $self->listen_server($server);
    warn "server: $server";

}

no Moose;
__PACKAGE__->meta->make_immutable;
