use strict;
use warnings;
use Test::More qw/no_plan/;
use Data::Dumper;

use lib 'lib';
use Grids::Test;

# how many clients to connect
my $count = 3;

# create a new node with some clients connected for testing
my ($node, @clients) = Grids::Test->node_with_many_clients($count, debug => 0);

# be notified of FooEvents
$_->register_hook('FooEvent' => \&got_foo) for @clients;

# brodcast a "FooEvent" to all connected clients
$c->do_request('Broadcast.Event', { foo => 'bar' });

my $got_foo = 0;
foreach my $client (@clients) {
    $got_foo++;
    ok($client->{got_foo}, "client received broadcast event");
}

is($got_foo, scalar @clients, "correct number of clients received event");

# broadcast a "BarEvent" using the _broadcast flag
my $bar_count = 0;
$_->register_hook('BarEvent' => { $bar_count++ }) for @clients;
$c->do_request('BarEvent', { 'bar' => 'baz', '_broadcast' => 1 });
is($bar_count, scalar @clients, "correct number of clients received event with broadcast flag");

sub got_foo {
    my ($client, $evt) = @_;

    $client->{got_foo} = 1;
    warn "got foo!";
}
