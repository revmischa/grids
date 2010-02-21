package Grids::Node;
use strict;
use warnings;

__PACKAGE__->register_hooks(
    'Broadcast.Event' => \&hook_broadcast_event,
);

sub hook_broadcast_event {
    my Grids::Node $node = shift;
    my Grids::Protocol::Event $evt = shift;

    $node->network_broadcast($evt);
    return $node->hook_ok;
}

1;
