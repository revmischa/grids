
# tests for high-level Grids client/server functionality
use Test::More tests => 17;

use strict;
use warnings;

use lib 'lib';
use Grids::Test;
use Grids::Node::Hooks::Authentication;

my $debug = 1;

my ($client, $server, $id, $server_id, $server_transport) = Grids::Test->client_server_pair(debug => $debug, auto_flush_queue => 1);

my $client_got_event;
$client->register_hook('Authentication.Login', \&client_login_hook);
$client->register_hook('Services.List', \&client_service_list);
$client->register_hook('Storage.List', \&client_storage_list);
$client->register_hook(qr//, sub { $client_got_event = 1 });

# test failed login
my $login_good = 0;
c_req_expect_response('Authentication.Login');

# test key auth (broken atm)
#$server->configuration->set_conf('Node.AuthorizedKeys', { $client->id => $id->pubkey->serialize });
$login_good = 1;
$client->login;
ok($client_got_event, "Client received Authentication.Login event");

ok($client->session_token, "Got session token");

c_req_expect_response('Services.List');

# test storage
my $storage_list = [];
{
    c_req_expect_response('Storage.List');

    c_req('Storage.Put', { key => 'foo', value => 'bar' });
    $storage_list = [ 'foo' ];
    c_req_expect_response('Storage.List');

    c_req('Storage.Empty');
    $storage_list = [];
    c_req_expect_response('Storage.List');
}

# attempt to verify shared secret
{
    my $question = 'smp_question';
    my $secret = 'smp_secret';

    $server->register_hook('OTR.SMPRequest', sub {
        my ($node, $evt) = @_;
        my $peer_name = $evt->peer_name;
        my $req_question = $evt->question;
        
        is($question, $req_question, "got correct SMP question in request");
    });

    $server->configuration->set_conf('SMP.Secret', $secret);
    $client->initiate_smp($secret, $question);
    flush();
}

sub flush {
    for (1..20) {
        c_flush();
        s_flush();
    }
}

sub s_flush {
    $server->flush_event_queue;
}

sub c_flush {
    $client->flush_event_queue;
}

sub c_req_expect_response {
    my ($evt_name, $args) = @_;
    c_req($evt_name, $args);
    ok($client_got_event, "Client got $evt_name response");
}

sub c_req {
    my ($evt_name, $args) = @_;
    $client_got_event = 0;
    $client->send_event($evt_name, $args);
    flush();
}

sub client_service_list {
    my ($c, $evt) = @_;

    ok($evt->services, "Services list");
    return 0;
}

sub client_storage_list {
    my ($c, $evt) = @_;

    is_deeply($evt->keys || [], $storage_list, "Storage.List");
    return 0;
}

sub client_login_hook {
    my ($c, $evt) = @_;

    is ($c, $client, "Got client in hook info");
    is ($evt->name, 'Authentication.Login', "Got correct event in hook info");

    if ($login_good) {
        is($evt->is_success, 1, 'Login successful');
    } else {
        is($evt->error, Grids::Node::Hooks::Authentication::ERROR_LOGIN_INVALID, 'Login unsuccessful');
    }

    return 1;
}
