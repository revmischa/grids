# tests for high-level Net client/server functionality
use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetNode;
use NetClient;

my $server = NetNode->new(debug => 1);
my $server_trans = $server->add_transport('Loop');

my $client = NetClient->new(id => '123456', transport => 'Loop', debug => 1);
my $client_trans = $client->transport;

# connect client to server using Loop transport
$client->connect($server_trans);

$client->do_request('Login', { dongs => 1});
