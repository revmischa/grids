use strict;
use warnings;

package Grids::Transport::TCP;
use base 'Grids::Transport';

use Carp qw/croak/;
use IO::Socket::INET;

sub new {
    my ($class, $parent, %opts) = @_;

    my $self = $class->SUPER::new($parent, %opts);
    $self->parent->conf->add_conf(port => 1488); # add default port

    $self->{sockets} = [];

    return $self;
}

sub connect {
    my ($self, $addr) = @_;
    
    return undef unless $addr;

    croak "must pass Grids::Address to Grids::Transport::TCP->connect"
        unless $addr->isa('Grids::Address');

    my $sock = IO::Socket::INET->new(Proto     => "tcp",
                                     Blocking  => 1,
                                     Reuse     => 1,
                                     PeerAddr  => $addr->address,
                                     PeerPort  => $addr->port,
                                     ) or return $self->error("Could not connect to host $addr: $!");

    $self->{sock} = $sock;
    $self->add_socket($sock);

    $self->outgoing_connection_established($sock);
}

sub write {
    my ($self, $data, $sock) = @_;

    my $datalen = length $data;
    my $datalen_packed = pack("N", $datalen);

    $sock ||= $self->{sock};

    if ($sock && $sock->connected) {
        my $byte_count = $sock->syswrite($datalen_packed . $data);
    } else {
        $self->error("Tried to send data [$data] to unconnected transport");
        return 0;
    }

    return 1;
}

sub listen_sock {
    my ($self) = @_;

    return $self->{listen_sock} = IO::Socket::INET->new(Proto     => "tcp",
                                                        Blocking  => 0,
                                                        Reuse     => 1,
                                                        LocalPort => $self->parent->conf->get_conf('port'),
                                                        Listen    => 16,
                                                        ) or die $!;
}

sub close_all {
    my ($self) = @_;
    $_->close foreach $self->sockets;
}

sub add_socket {
    my ($self, $socket) = @_;
    return if grep { $_ eq $socket } $self->sockets;
    push @{$self->{sockets}}, $socket;
    $self->add_to_read_set($socket);
}

sub remove_socket {
    my ($self, $socket) = @_;
    $self->{sockets} = [ grep { $_ ne $socket } $self->sockets ];
    $self->remove_from_read_set($socket);
}

sub sockets { @{$_[0]->{sockets}} }

sub add_to_read_set {
    my ($self, $socket) = @_;

    unless ($self->{read_set}) {
        my $read_set = IO::Select->new;
        $self->{read_set} = $read_set;
    }

    $self->{read_set}->add($socket);
}

sub remove_from_read_set {
    my ($self, $socket) = @_;
    $self->{read_set}->remove($socket);
}

sub listen {
    my $self = shift;

    # create main listening socket and add it to our read_set
    my $ls = $self->listen_sock;
    $self->add_socket($ls);
    $ls->listen;
}

sub select {
    my ($self, $timeout) = @_;

    $timeout ||= 1;

    my ($rh_set) = IO::Select->select($self->{read_set}, undef, undef, $timeout);
    foreach my $rh (@$rh_set) {

        if ($rh eq $self->{listen_sock}) {
            # this is the main read socket, accept the connection
            # and add it to the read set
            my $ns = $rh->accept;
            $self->add_socket($ns);
            $self->incoming_connection_established($ns);
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
                    if ($read != $incoming_len) { # o shit
                        warn "Message received did not match expected length $incoming_len";
                    } else {
                        # chill, got out message, process it
                        $self->data_received($rh, $buf);
                    }
                }
            } else {
                # socket is closed
                $self->remove_socket($rh);
                $rh->close;
            }
        }
    }

    return 0;
}

1;
