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

$client->register_event_hook('Login', \&login_hook);
$client->do_request('Login', { dongs => 1 });

sub login_hook {
    my ($c, $args) = @_;

    is($args->{error}, '-1', 'Login protocol');
}
