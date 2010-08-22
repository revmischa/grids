package Grids::Node::Hooks::Broadcast;

use Moose;
use Grids::Node;

Grids::Node->register_hooks(
    'Broadcast.Event' => \&hook_broadcast_event,
    qr/.*/ => \&process_broadcast_flag,
);

use constant {
    BROADCAST_ERROR_NO_EVENT_NAME => -1,
};

# check all events and see if they have _broadcast=1, if so then
# rebroadcast them to all connected clients
sub process_broadcast_flag {
    my ($node, $evt) = @_;

    return unless $evt->is_broadcast;

    # clone evt and broadcast it
    my $clone = $evt->clone_cleaned;
    $clone->clear_broadcast_flag;
    $node->network_broadcast($clone);

    return;
}

# explicit request to broadcast an event to all connected clients
# currently broken, since we can't arbitrarily attach args to an event
sub hook_broadcast_event {
    my ($node, $evt) = @_;

    # dont broadcast broadcast acks
    return if $evt->is_ack;

    # what event should be broadcast?
    my $event_name = $evt->name;

    # we should return a hook error here, but since hook_error returns
    # with the same event (BroadcastEvent) we can end up in an
    # infinite loop
    unless ($event_name) {
        $node->warn('broadcast requested but no event_name specified');
        return;
    }

    # clone event
    my $new_evt = $evt->clone;
    $new_evt->set_event($event_name);

    warn "new evt name: " . $new_evt->name;
    $node->network_broadcast($new_evt);
    return $node->hook_ok(base => { ack => 1 });
}

no Moose;
__PACKAGE__->meta->make_immutable;

