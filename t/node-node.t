#!/usr/bin/perl

# This is a script to test Node to Node communication

use strict;
use warnings;
use Test::More qw/no_plan/;
use Data::Dumper;

use lib 'lib';
use Grids::Node;
use Grids::Identity;

my $debug = 0;

my $nodecount = 3;
my $connections = 0;
my @nodes;

init_nodes();

sub init_nodes {
    my %loopmap; # node -> loop

    for (1 .. $nodecount) {
        # generate an identity for this new node
        my $id = Grids::Identity->create_for_test(name => "test-node$_");

        $id->set_callback('error', sub { my ($otr, $user, $proto, $peer, $error) = @_; warn "[$user] OTR error: $peer - $error" });
        $id->set_callback('warning', sub { my ($otr, $user, $proto, $peer, $warn) = @_; warn "[$user] OTR warning: $peer - $warn" });
        $id->set_callback('info', sub { my ($otr, $user, $proto, $peer, $info) = @_; warn "[$user] OTR info: $peer - $info" });
        $id->set_callback('otr_message', sub { my ($otr, $user, $proto, $peer, $notif) = @_; warn "[$user] OTR system message: $peer - $notif" });

        # create a new node
        my $node = Grids::Node->new(debug => $debug, id => $id);

        # set node-node private key
        $node->configuration->set_conf('Node.PrivateKey' => '123');

        # handle connections and node-node communication
        $node->register_hook('Connected', \&node_connected);
        $node->register_hook('Login', \&login);
        $node->register_hook('Error', \&node_error);

        my $loop = $node->add_transport('Loop');
        $loopmap{$node} = $loop;
        push @nodes, $node;
    }

    # connect all loops
    for (my $i = 0 ; $i < $nodecount ; $i++) {
        my $nextnode = $i != $nodecount - 1 ? $nodes[$i + 1] : $nodes[0];

        my $loop = $loopmap{$nodes[$i]};
        my $nextloop = $loopmap{$nextnode};

        $loop->connect($nextloop);
    }

    flush();
    flush();
    flush();
    flush();

    is($connections, $nodecount * 2, "all nodes connected");

    flush();
    flush();
    flush();
}


# process all waiting events
sub flush {
    $_->flush_event_queue foreach @nodes;
}

# node got error
sub node_error {
    my ($node, $info) = @_;
    not_ok(Dumper($info));
}

# send node key (FIXME: make encrypted)
sub node_connected {
    my ($node, $event) = @_;

    my $connection = $event->connection;
    ok($connection, "node connected (" . $connection->id->name . " <-> " . $connection->peer->name . ")");
    $connections++;

    # try to log in with shared node privkey
    return { event => 'Login', node_private_key => $node->configuration->get('Node.PrivateKey') };
}
    
# node-node login
sub login {
    my ($node, $info) = @_;

    ok(%$info, "node-node login");

    return;
}
