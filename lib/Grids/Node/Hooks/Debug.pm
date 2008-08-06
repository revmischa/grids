package Grids::Node::Hooks::Debug;

use strict;
use warnings;
use Grids::Node;

Grids::Node->register_hooks(
                            "Debug.Warn" => \&warn,
                            );

sub warn {
    my ($node, $info) = @_;

    $node->warn("\nGot Debug.Warn: $info->{args}->{message}\n");
}


1;
