# tests for high-level Net client/server functionality
use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetNode;
use NetClient;

my $debug = 1;

my $server = NetNode->new(debug => $debug);
my $server_trans = $server->add_transport('Loop');

my $client = NetClient->new(id => '123456', transport => 'Loop', debug => $debug);

# connect client to server using Loop transport
$client->connect($server_trans);

$client->register_event_hook('Authentication.Login', \&client_login_hook);
$client->register_event_hook('Services.List', \&client_service_list);
$client->register_event_hook('Storage.List', \&client_storage_list);

my $login_good = 0;
$client->do_request('Authentication.Login', { public_key => 'lolwtf' });

$server->conf->set_conf('Node.AuthorizedKeys', { 'lolwtf' => 1 });
$login_good = 1;
$client->do_request('Authentication.Login', { public_key => 'lolwtf' });

ok($client->session_token, "Got session token");

$client->do_request('Services.List');

my $storage = {};
$client->do_request('Storage.List');
$storage->{lol} = 'dongs';
$client->do_request('Storage.Store', $storage);
$client->do_request('Storage.List');

sub client_service_list {
    my ($c, %info) = @_;
    ok($info{args}{services}, "Services list");
}

sub client_storage_list {
    my ($c, %info) = @_;
    use Data::Dumper;
    warn Dumper(\%info);

    is_deeply($info{args}{storage}, $storage, "Storage list");
}

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
