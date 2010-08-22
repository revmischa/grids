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

    return undef unless $evt->is_broadcast;
    $evt->clear_broadcast_flag;
    $node->network_broadcast($evt);

    return undef;
}

# explicit request to broadcast an event to all connected clients
# broadcasts a copy of the event with the event name specified in $args->{event_name}
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
    my $new_evt = bless { %$evt }, ref $evt;
    $new_evt->event_name($event_name);
    $new_evt->clear_id;

    # copy args
    my %args = %{$evt->args};
    $new_evt->args(\%args);

    $node->network_broadcast($new_evt);
    return $node->hook_ok(base => { ack => 1 });
}

no Moose;
__PACKAGE__->meta->make_immutable;

