
# tests for high-level Grids client/server functionality
use Test::More tests => 25;

use strict;
use warnings;

use lib 'lib';
use Grids::Test;
use Grids::Node::Hooks::Authentication;

my ($client, $server, $id, $server_id, $server_transport) = Grids::Test->client_server_pair(auto_flush_queue => 1);

my $client_got_event;
my $server_got_event;
my $encrypted = 0;
$client->register_hook('Authentication.Login', \&client_login_hook);
$client->register_hook('Services.List', \&client_service_list);
$client->register_hook('Storage.List', \&client_storage_list);
$client->register_hook(qr//, sub { $client_got_event = 1; return; });
$server->register_hook(qr//, sub { $server_got_event = 1; return; });
$client->register_hook('Encrypted', sub { $encrypted = 1; return; });

# connect
$client->connect($server_transport);

# test failed login
my $login_good = 0;
c_req_expect_response('Authentication.Login');

# we should now be encrypted
is($encrypted, $client->use_encryption || 0, "Encryption status=$encrypted");

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
    # TODO: continue SMP
}

# upload a simple program and execute it
{
    # simple program that stores a value in the client's storage
    my $prog_key = 'cs-storage-key';
    my $prog_str = ' - CLIENT SERVER STORAGE TEST PROGRAM - ';
    my $prog_src = qq/
#define SYSCALL_PUT 600

    .rdata
csteststring: .ascii "$prog_str"
stringlength: .w . - csteststring
cstestkey: .ascii "$prog_key"
keylength: .w . - cstestkey

    .text
.globl main
main:
    ; save string in storage
    la \$a0, cstestkey
    lw \$a1, keylength
    la \$a2, csteststring
    lw \$a3, stringlength
    li \$v0, SYSCALL_PUT
    syscall
/;

    # upload program
    c_req('Storage.Put', { key => 'testprogsrc', value => $prog_src });
    
    # tell node to load program from storage, compile it, and execute
    c_req('Program.CompileAndExecute', { source_storage_key => 'testprogsrc' });

    # should now have program and prog_str in storage
    $storage_list = [ 'testprogsrc', $prog_key ];
    c_req_expect_response('Storage.List');
}

sub c_req_expect_response {
    my ($evt_name, $args) = @_;
    c_req($evt_name, $args);
    ok($client_got_event, "Client got $evt_name response");
}

sub c_req {
    my ($evt_name, $args) = @_;
    $client_got_event = 0;
    $server_got_event = 0;
    $client->send_event($evt_name, $args);
    ok($server_got_event, "Server got $evt_name event");
}

sub client_service_list {
    my ($c, $evt) = @_;

    ok($evt->services, "Services list");
    return;
}

sub client_storage_list {
    my ($c, $evt) = @_;

    is_deeply($evt->keys || [], $storage_list, "Storage.List");
    return;
}

sub client_login_hook {
    my ($c, $evt) = @_;

    is ($c, $client, "Got client in hook info");
    is ($evt->name, 'Authentication.Login', "Got correct event in hook info");

    if ($login_good) {
        is($evt->is_success, 1, 'Login successful');
    } else {
        is($evt->error, 'Error.Authentication.LoginInvalid', 'Login unsuccessful');
    }

    return;
}
