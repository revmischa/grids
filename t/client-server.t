
# tests for high-level Grids client/server functionality
use Test::More qw(no_plan);

use strict;
use warnings;

use lib 'lib';
use Grids::Test;
use Grids::Node::Hooks::Authentication;

my $debug = 1;

my ($client, $server, $id, $server_id, $server_transport) = Grids::Test->client_server_pair(debug => $debug, auto_flush_queue => 1);

$client->register_hook('Authentication.Login', \&client_login_hook);
$client->register_hook('Services.List', \&client_service_list);
$client->register_hook('Storage.List', \&client_storage_list);

my $login_good = 0;

c_req('Authentication.Login');

#$server->configuration->set_conf('Node.AuthorizedKeys', { $client->id => $id->pubkey->serialize });
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


# attempt to verify shared secret
my $question = 'smp_question';
my $secret = 'smp_secret';

$server->register_hook('SMP.Request', sub {
    my ($node, $evt) = @_;
    my $peer_name = $evt->args->{peer_name};
    my $req_question = $evt->args->{question};

    is($question, $req_question, "got correct SMP question in request");
});


$server->configuration->set_conf('SMP.Secret', $secret);
$client->initiate_smp($secret, $question);
flush();


sub flush {
    for (1..20) {
        c_do();
        s_do();
    }
}

sub s_do {
    $server->flush_event_queue;
}

sub c_do {
    $client->flush_event_queue;
}
sub c_req {
    $client->send_event(@_) if @_;
    s_do();
    $client->flush_event_queue;
}

sub client_service_list {
    my ($c, $evt) = @_;
    ok($evt->args->{services}, "Services list");
    return 0;
}

sub client_storage_list {
    my ($c, $evt) = @_;

    is_deeply($evt->args->{storage}, $storage, "Storage list");
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
