package Grids::Node::Hooks::Debug;

use strict;
use warnings;
use Grids::Node;

Grids::Node->register_hooks(
                            "Debug.Warn" => \&warn,
                            "Debug.Echo" => \&echo,
                            );

sub warn {
    my ($node, $info) = @_;

    $node->warn("\nGot Debug.Warn: $info->{args}->{message}\n");
    return undef;
}

sub echo {
    my ($node, $info) = @_;

    $node->warn("\nGot Debug.Echo: $info->{args}->{message}\n");
    return $node->event_hook_success(%{$info->{args}});
}


1;
