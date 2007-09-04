use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetProtocol;

my %EVENTS; # holds event info received in Protocol event handler callback

my $p_cli = NetProtocol->new(encapsulation => 'JSON');

my $initiation = $p_cli->initiation_string;

my $p_srv = NetProtocol->new_from_initiation_string($initiation);

ok($p_srv, "Created new NetProtocol from initiation string");

# test encapsulation / parsing
{
    my $req = $p_cli->encapsulate('Login', {
        public_key => '123456',
    });

    my $evt = $p_srv->parse_request($req);

    is($evt->event_name, 'Login', 'Correct event type');
    is($evt->args->{public_key}, '123456', 'Correct event args');
}

