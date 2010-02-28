package Grids::Transport::TCP::AnyEvent;

use Moose;
    extends 'Grids::Transport::TCP';
    with 'Grids::Transport::Driver';

use Carp qw/croak/;
use AnyEvent;
use AnyEvent::Socket;
use AnyEvent::Handle;

use Grids::Protocol::Connection::TCP::AnyEvent;
use Grids::Address::IPv4;

has 'listen_server' => (
    is => 'rw',
);

has connections => (
    is => 'rw',
    lazy => 1,
    isa => 'ArrayRef',
    default => sub { [] },
);

# how many bytes to read at a time
our $READ_SIZE = 1450;

sub disconnect {
    my ($self, $connection) = @_;

    $self->reset($connection);
}

sub connect {
    my ($self, $addr) = @_;
    
    return undef unless $addr;

    croak "must pass Grids::Address to Grids::Transport::TCP->connect"
        unless $addr->does('Grids::Address');

    my $host = $addr->address;
    my $port = $addr->port;

    my $client = tcp_connect $host, $port, sub {
        my ($fh, $connected_host, $connected_port) = @_;

        # create a condvar to receive read events
        my $res_cv = AnyEvent->condvar;

        # create connection instance
        my $conn = Grids::Protocol::Connection::TCP::AnyEvent->new(
            transport => $self,
            channel => $fh,
            inbound => 0,
        );
        push @{$self->connections}, $conn;

        my $handle = $self->connection_handle($fh, $connected_host, $connected_port, $conn, $res_cv);

        $res_cv->cb(sub {
            my $data = $res_cv->recv;  # non-blocking read

            if (defined $data) {
                $self->data_received($conn, $data);
            } else {
                warn "didn't receive data";
            }
        });

        $self->outgoing_connection_established($conn);
    };

    return $client;
}

sub write {
    my ($self, $data, $connection) = @_;

    my $channel = $connection->channel
        or croak "Tried to write on a connection without a channel";

    # first, 4 byte message length marker
    my $data_length = length $data or return;
    if ($data_length > 2**32) {
        croak "your message is too long! message fragmentation not implemented yet";
    }

    return syswrite $channel, pack("N", $data_length) . $data;
}

after close_server => sub {
    my $self = shift;

    $self->listen_cv(undef);
    $self->listen_server(undef);
};

after close_all_clients => sub {
    my ($self) = @_;

    # delete references to connections
    $self->connections([]);
};

before reset => sub {
    my ($self, $connection) = @_;

    $self->remove_connection($connection);
};

# given a connection, delete it from the list of client sockets
sub remove_connection {
    my ($self, $connection) = @_;

    unless ($connection) {
        Carp::cluck "remove_connection called with undefined connection";
        $self->disconnected;
        return;
    }

    # get rid of our reference to this connection
    my $connections = $self->connections;
    my @new_conns = grep { $_ && $_ != $connection } @$connections;
    $self->connections(\@new_conns);

    #warn "removed " . (@$connections - @new_conns) . " connections";

    # done reading
    $connection->clear_handle;

    # dispatch disconnected event
    $self->disconnected($connection);
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

        # create connection instance
        my $conn = Grids::Protocol::Connection::TCP::AnyEvent->new(
            transport => $self,
            channel => $fh,
            inbound => 1,
        );
        
        # keep track of connection
        push @{$self->connections}, $conn;

        # create a handle for this connection
        my $handle = $self->connection_handle($fh, $host, $port, $conn, $res_cv);

        $res_cv->cb(sub {
            my $data = $res_cv->recv;  # non-blocking read

            if (defined $data) {
                $self->data_received($conn, $data);
            } else {
                warn "didn't receive data";
            }
        });

        # send connected event
        $self->incoming_connection_established($conn);
    };

    # keep track of the server until we don't want this socket around anymore
    $self->listen_server($server);
}

sub connection_handle {
    my ($self, $fh, $connected_host, $connected_port, $conn, $res_cv) = @_;

    my $handle = new AnyEvent::Handle(
        fh => $fh,
        on_error => sub {
            my ($handle) = @_;                

            $self->reset($conn);
            $handle->destroy;
            $res_cv->send;
        },
        on_eof => sub {
            my ($handle) = @_;                

            $self->reset($conn);
            $handle->destroy;
            $res_cv->send;
        },
    );

    # create a read watcher
    my $read_handler = $self->read_handler($fh, $connected_host, $connected_port, $res_cv);
    $handle->on_read($read_handler);

    $conn->handle($handle);
}

sub read_handler {
    my ($self, $fh, $host, $port, $res_cv) = @_;

    my $read_handler = sub {
        my ($handle) = @_;

        # read in one 32-bit longword (message length)
        my $read = 0;

        $handle->push_read(chunk => 4, sub {
            my (undef, $data) = @_;

            # $data should contain a long in network byte order telling us how long the rest of 
            # this message is, followed by a comma
            my $incoming_len = unpack("N", $data);

            return unless $incoming_len;

            # read in the message
            $handle->push_read(chunk => $incoming_len, sub {
                my (undef, $message) = @_;
                $res_cv->send($message);
            });
        });
    };

    return $read_handler;
}

no Moose;
__PACKAGE__->meta->make_immutable;
