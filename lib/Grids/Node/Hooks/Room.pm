package Grids::Node::Hooks::Room;

use strict;
use warnings;
use Grids::Node;
use Grids::Room;
use Data::UUID;

my $prefix = 'Room';

Grids::Node->register_hooks(
                            "$prefix.Create" => \&create,
                            "$prefix.List" => \&list_rooms,
                            );

our %ROOMS;

sub create {
    my ($node, $args) = @_;

    my $room = Grids::Room->create;

    $ROOMS{$room->id} = $room;

    return $node->event_hook_success(id => $room->id);
}

sub list_rooms {
    my ($node, $args) = @_;

    return $node->event_hook_success(rooms => [ keys %ROOMS ]);
}

1;
