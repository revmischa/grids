#!/usr/bin/perl

use strict;
use warnings;
use Test::More qw/no_plan/;
use Data::Dumper;

use lib 'lib';
use Grids::Test;

# how many clients to connect
my $count = 3;

my $debug = 1;

# create a new node with some clients connected for testing
my ($node, @clients) = Grids::Test->node_with_many_clients($count, debug => $debug, use_encryption => 0);

my $got_foo = {};

# be notified of FooEvents
foreach my $c (@clients) {
    $c->register_hook('Echo' => \&got_foo);
}


flush() for 1..10;

# brodcast an "Echo" to all connected clients
$clients[0]->send_broadcast_event('Echo');

flush() for 1..20;

my $got_foo_count = 0;
foreach my $client (@clients) {
    $got_foo_count++ if $got_foo->{$client};
    ok($got_foo->{$client}, "client " . $client->name . " received broadcast event");
}

is($got_foo_count, scalar @clients, "correct number of clients received event");

# broadcast a "BarEvent" using the is_broadcast flag
my $bar_count = 0;
$_->register_hook('Echo' => sub { $bar_count++ }) for @clients;

my $evt = $clients[0]->connection->construct_event('Echo');
$evt->set_broadcast_flag;
$clients[0]->send_event($evt);

flush() for 1..10;

is($bar_count, scalar @clients, "correct number of clients received event with broadcast flag");

sub got_foo {
    my ($client, $evt) = @_;

    $got_foo->{$client} = 1;
}

sub flush {
    $node->flush_event_queue;
    $_->flush_event_queue foreach @clients;
}
