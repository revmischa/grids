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

my $nodecount = 4;
my $connections = 0;
my $encrypted_connections = 0;
my $connection_attempts;
my @nodes;

init_nodes();

sub init_nodes {
    my %loopmap; # node -> loop

    for (1 .. $nodecount) {
        # generate an identity for this new node
        my $id = Grids::Identity->create_for_test(name => "test-id-$_");

        $id->set_callback('error', sub { my ($otr, $user, $proto, $peer, $error) = @_; warn "[$user] OTR error: $error\n" });
        $id->set_callback('warning', sub { my ($otr, $user, $proto, $peer, $warn) = @_; warn "[$user] OTR warning:  $warn\n" });
        $id->set_callback('info', sub { my ($otr, $user, $proto, $peer, $info) = @_; warn "[$user] OTR info:  $info\n" });
        $id->set_callback('otr_message', sub { my ($otr, $user, $proto, $peer, $notif) = @_; warn "[$user] OTR system message: $notif" });

        # create a new node
        my $node = Grids::Node->new(debug => $debug, id => $id, use_encryption => 0, transport_driver => 'Loop');

        # set node-node private key
        $node->configuration->set_conf('Node.PrivateKey' => '123');

        # handle connections and node-node communication
        $node->register_hook('Connected', \&node_connected);
        $node->register_hook('Encrypted', \&connection_encrypted);
        $node->register_hook('Node.Login', \&login);
        $node->register_hook('Error', \&node_error);

        push @nodes, $node;
    }

    # connect all loops
    my @connections;
    for (my $i = 0 ; $i < $nodecount ; $i++) {
        for (my $j = $i+1 ; $j < $nodecount ; $j++) {
            my $node = $nodes[$i];
            my $nextnode = $nodes[$j];

            ok(1, "connecting " . ($i+1) . "[$node] <- - -> " . ($j + 1) . "[$nextnode]");

            my $conn = $node->new_transport->connect($nextnode->new_transport);

            push @connections, $conn;

            $connection_attempts++;
        }
    }
 
    is($connections, $connection_attempts, "all nodes connected");

    # test encrypted messaging
    {
        # turn on encryption
        $_->enable_encryption foreach @nodes;

        # establish encrypted connections
        $_->activate_encryption foreach @nodes;

        is($encrypted_connections, $connection_attempts, "encrypted sessions started");
    }

    # disconnect all loops
    $_->disconnect_all_clients for @nodes;
}

# node got error
sub node_error {
    my ($node, $info) = @_;
    not_ok(Dumper($info));
}

sub connection_encrypted {
    my ($node, $event) = @_;

    my $connection = $event->connection;
    $encrypted_connections++;
    ok($connection, "node $node encrypted($connection) (" . $connection->id->name . " <---> " . $connection->peer->name . ")");
}

sub node_connected {
    my ($node, $event) = @_;

    my $connection = $event->connection;
    ok($connection, "node $node connected($connection) (" . $connection->id->name . " <---> " . $connection->peer->name . ")");
    $connections++;

    # try to log in with shared node privkey
    # FIXME: catch this
    return { event => 'Node.Login', node_private_key => $node->configuration->get('Node.PrivateKey') };
}
    
# node-node login
sub login {
    my ($node, $info) = @_;

    ok(%$info, "node-node login");

    return;
}
