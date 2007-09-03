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

$client->register_event_hook('Authentication.Login', \&client_login_hook);

my $login_good = 0;
$client->do_request('Authentication.Login', { public_key => 'lolwtf' });

$server->conf->set_conf('Node.AuthorizedKeys', { 'lolwtf' => 1 });
$login_good = 1;
$client->do_request('Authentication.Login', { public_key => 'lolwtf' });

sub client_login_hook {
    my ($c, %info) = @_;

    my $args = $info{args};
    is ($c, $client, "Got client in hook info");
    is ($info{event}, 'Authentication.Login', "Got correct event in hook info");

    if ($login_good) {
        is($args->{success}, 1, 'Login successful');
    } else {
        is($args->{error}, NetNode::ERROR_LOGIN_INVALID, 'Login unsuccessful');
    }

    return 1;
}
