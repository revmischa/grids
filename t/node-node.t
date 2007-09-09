#!/usr/bin/perl

# This is a script to test Node to Node communication

use strict;
use warnings;
use Test::More qw/no_plan/;

use lib 'lib/perl';
use NetNode;

my $debug = 0;

my $nodecount = 3;
my @nodes;

init_nodes();

sub init_nodes {
    my %loopmap; # node -> loop

    for (1 .. $nodecount) {
        my $node = NetNode->new(debug => $debug);
        $node->conf->set_conf('Node.PrivateKey' => '123');
        $node->register_event_hook('ProtocolEstablished', \&node_connected);
        $node->register_node_protocol_handler;
        my $loop = $node->add_transport('Loop');
        $loopmap{$node} = $loop;
        push @nodes, $node;
    }

    # connect all loops
    foreach my $loop (values %loopmap) {
        foreach my $node (@nodes) {
            next if $loopmap{$node} eq $loop;
            $loop->connect($loopmap{$node});
        }
    }

    flush();
}

sub flush {
    1 while (grep { $_->flush_event_queue } @nodes);
}

sub node_connected {
    my ($node, %info) = @_;

    ok(%info, "node $node connected");
    return { event => 'Login', node_private_key => $node->conf->get('Node.PrivateKey') };
}
    
