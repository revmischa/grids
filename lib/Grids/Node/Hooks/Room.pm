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
							"$prefix.ListObjects" => \&list_objects,
                            );

our %ROOMS;
our %OBJECTS;
our %PEOPLE;

sub create {
    my ($node, $evt) = @_;

    my $room = Grids::Room->create;

    $ROOMS{$room->id} = $room;

    return $node->hook_ok(id => $room->id);
}

sub get_room {
    my ($evt) = @_;

    my $roomid = $evt->args->{room_id}
        or return undef;

    return $ROOMS{$roomid};
}


########


sub list_rooms {
    my ($node, $evt) = @_;

    return $node->hook_ok(rooms => [ keys %ROOMS ]);
}

# args: room_id
sub list_objects_in_room {
    my ($node, $evt) = @_;

    my $room = get_room($evt)
        or return $node->hook_error("Invalid room_id");

    my $objects = $room->objects;
    return $node->hook_ok(objects => [ keys %$objects ]);
}

sub object_hook_ok {
    my ($self, $node, $object, $evt, %args) = @_;

    my $room = get_room($evt)
        or return $node->hook_error("Invalid room_id");

    return $node->hook_ok(id => $object->id, req => $evt->args, %args);
}

# args: room_id, [\%attr]
sub create_object {
    my ($node, $evt) = @_;

    my $room = get_room($evt)
        or return $node->hook_error("Invalid room_id");

    my $object = $room->create_object($evt->args->{attr});
    $OBJECTS{$object->id} = $object;

    return __PACKAGE__->object_hook_ok($node, $object, $evt);
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

    return __PACKAGE__->object_hook_ok($node, $obj, $evt);
}

1;
