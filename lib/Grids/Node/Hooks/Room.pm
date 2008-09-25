package Grids::Node::Hooks::Room;

use strict;
use warnings;
use Grids::Node;
use Grids::Room;
use Grids::Room::Object;

use Data::UUID;

my $prefix = 'Room';

Grids::Node->register_hooks(
                            "$prefix.Create" => \&create,
                            "$prefix.List" => \&list_rooms,
                            "$prefix.CreateObject" => \&create_object,
                            "$prefix.UpdateObject" => \&update_object,
                            );

our %ROOMS;
our %OBJECTS;
our %PEOPLE;

sub create {
    my ($node, $evt) = @_;

    my $room = Grids::Room->create;

    $ROOMS{$room->id} = $room;

    return $node->event_hook_success(id => $room->id);
}

sub list_rooms {
    my ($node, $evt) = @_;

    return $node->event_hook_success(rooms => [ keys %ROOMS ]);
}

# args: room_id, [\%attr]
sub create_object {
    my ($node, $evt) = @_;

    my $roomid = $evt->args->{room_id}
       or return $node->event_hook_error('No room_id specified');

    my $room = $ROOMS{$roomid}
       or return $node->event_hook_error("Invalid room_id: $roomid");

    my $object = $room->create_object($evt->args->{attr});
    $OBJECTS{$object->id} = $object;

    return $node->event_hook_success(id => $object->id);
}

# args: id, \%new_attr
sub update_object {
    my ($node, $evt) = @_;

    my $objid = $evt->args->{id}
       or return $node->event_hook_error('No id specified');

    my $obj = $OBJECTS{$objid}
       or return $node->event_hook_error("Invalid id: $objid");

    my $new_attr = $evt->args->{attr} || {};

    # for now just replace attr, in future should only update keys that exist in $new_attr
    $obj->attr($new_attr);

    return $node->event_hook_success;
}

1;
