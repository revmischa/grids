use strict;
package NetTransport::TCP;
use base 'NetTransport';
use IO::Socket::INET;

sub new {
    my ($class, $parent, %opts) = @_;

    my $self = $class->SUPER::new($parent, %opts);
    $self->parent->conf->add_conf(port => 1488);

    return $self;
}

sub accept_loop {
    my ($self) = @_;

    warn "accept loop pid: $$\n";

    $self->{sock} = IO::Socket::INET->new(Proto     => "tcp",
                                          Blocking  => 1,
                                          Reuse     => 1,
                                          LocalPort => $self->node->conf->get_conf('port'),
                                          Listen    => 5,
                                          ) or die;

    while (my $con = $self->{sock}->accept) {
        warn "connection established";
        $self->connection_established($con);

        while (<$con>) {
            print "$_\n";
        }
    }

    return 0;
}

1;
