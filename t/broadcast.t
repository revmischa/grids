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
    $c->register_hook('FooEvent' => \&got_foo);
}


flush() for 1..10;

# brodcast a "FooEvent" to all connected clients
$clients[0]->do_request(event_args => { foo => 'bar', event_name => "FooEvent" }, event_name => 'Broadcast.Event');

flush() for 1..20;

my $got_foo_count = 0;
foreach my $client (@clients) {
    $got_foo_count++ if $got_foo->{$client};
    ok($got_foo->{$client}, "client " . $client->name . " received broadcast event");
}

is($got_foo_count, scalar @clients, "correct number of clients received event");

# broadcast a "BarEvent" using the _broadcast flag
my $bar_count = 0;
$_->register_hook('BarEvent' => sub { $bar_count++ }) for @clients;
$clients[0]->do_request(event_name => 'BarEvent', event_args => { 'bar' => 'baz', '_broadcast' => 1 });

flush() for 1..10;

is($bar_count, scalar @clients, "correct number of clients received event with broadcast flag");

sub got_foo {
    my ($client, $evt) = @_;

    $got_foo->{$client} = $evt->args->{foo};
}

sub flush {
    $node->flush_event_queue;
    $_->flush_event_queue foreach @clients;
}
