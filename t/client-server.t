# tests for high-level Grids client/server functionality
use strict;
use Test::More qw(no_plan);
use lib 'lib';
use Grids::Node;
use Grids::Client;
use Grids::Identity;

my $debug = 0;

my $srv_id = Grids::Identity->create_for_test;
my $server = Grids::Node->new(debug => $debug, id => $srv_id);
my $server_trans = $server->add_transport('Loop');

my $id = Grids::Identity->create_for_test;
my $client = Grids::Client->new(id => $id, transport => 'Loop', debug => $debug);

# connect client to server using Loop transport
$client->connect($server_trans);

$client->register_hook('Authentication.Login', \&client_login_hook);
$client->register_hook('Services.List', \&client_service_list);
$client->register_hook('Storage.List', \&client_storage_list);

my $login_good = 0;

c_req('Authentication.Login');

$server->conf->set_conf('Node.AuthorizedKeys', { $client->id => $id->pubkey->serialize });
$login_good = 1;
$client->login;
c_req();

ok($client->session_token, "Got session token");

c_req('Services.List');
c_req('Storage.Empty');

my $storage = {};
c_req('Storage.List');

$storage->{lol} = 'dongs';
c_req('Storage.Store', { data => $storage });
c_req('Storage.List');


sub s_do {
    $server->flush_event_queue;
}

sub c_req {
    $client->do_request(@_) if @_;
    s_do();
    $client->flush_event_queue;
}

sub client_service_list {
    my ($c, $info) = @_;
    ok($info->{args}{services}, "Services list");
    return 0;
}

sub client_storage_list {
    my ($c, $info) = @_;

    is_deeply($info->{args}{storage}, $storage, "Storage list");
    return 0;
}

sub client_login_hook {
    my ($c, $info) = @_;

    my $args = $info->{args};
    is ($c, $client, "Got client in hook info");
    is ($info->{event}, 'Authentication.Login', "Got correct event in hook info");

    if ($login_good) {
        is($args->{success}, 1, 'Login successful');
    } else {
        is($args->{error}, Grids::Node::ERROR_LOGIN_INVALID, 'Login unsuccessful');
    }

    return 1;
}
