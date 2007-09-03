# tests for high-level Net client/server functionality
use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetNode;
use NetClient;

my $server = NetNode->new(debug => 1);
my $server_trans = $server->add_transport('Loop');

my $client = NetClient->new(id => '123456', transport => 'Loop', debug => 1);

# connect client to server using Loop transport
$client->connect($server_trans);

$client->register_event_hook('Login', \&client_login_hook);
$server->register_event_hook('Login', \&server_login_hook);

$client->do_request('Login', { dongs => 1 });

sub client_login_hook {
    my ($c, %info) = @_;

    my $args = $info{args};
    is ($c, $client, "Got client in hook info");
    is ($info{event}, 'Login', "Got correct event in hook info");

    is($args->{error}, '-1', 'Login protocol');

    return 1;
}

sub server_login_hook {
    my ($s, %info) = @_;

    my $args = $info{args};
    is ($s, $server, "Got server in hook info");
    is ($info{event}, 'Login', "Got correct event in hook info");

    return {
        error => -1,
    };
}
