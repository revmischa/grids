use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetProtocol;

my %EVENTS; # holds event info received in Protocol event handler callback

my $p_cli = NetProtocol->new(
                             encapsulation => 'JSON',
                             event_handler => \&handler,
                             );

my $initiation = $p_cli->initiation_string;

my $p_srv = NetProtocol->new_from_initiation_string(
                                                    $initiation,
                                                    event_handler => \&handler,
                                                    );

# test encapsulation
my $req = $p_cli->encapsulate('Login', {
    public_key => '123456',
});

$p_srv->handle_request($req);

my $info = $EVENTS{'Login'};

is($info->{public_key}, '123456', 'Request');

# the "Server" side of the Protocol
sub handler {
    my ($proto, $event, $info) = @_;
    $EVENTS{$event} = $info;
}
