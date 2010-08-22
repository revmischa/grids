package Grids::Protocol::Serializer;

use Moose::Role;

use namespace::autoclean;
use Scalar::Util qw/blessed/;

requires qw/serialize deserialize/;

around 'deserialize' => sub {
    my ($orig, $self, $data) = @_;

    # lame hack to strip out inline unencrypted warnings from OTR (it makes the message unparsable)
    my $was_unencrypted = $data =~ s/^<b>The following message received from \S+ was <i>not<\/i> encrypted:<\/b>//;

    my $evt = $self->$orig($data);
    
    $evt->was_encrypted(0) if $was_unencrypted;
    
    return $evt;
};

# takes $event, $args
# returns appropriate Message class instance
sub construct_event {
    my ($self, $event, $args) = @_;

    # check if we got passed an event instance or an event name
    if (! blessed $event) {
        # construct event object
        $args ||= {};

        my $message_class = eval { $self->get_message_class($event); };
        unless ($message_class) {
            warn "Unknown message type $event";
            return;
        }

        $event = $message_class->new({
            base => {
                event => $event,
            },
            %$args,
        });
        Grids::Protocol::Event->meta->apply($event);
    }

    return $event;
}

# turn "Foo.Bar" or "Foo::Bar" into "Grids::Message::Foo::Bar"
sub get_message_class {
    my ($self, $msg_class) = @_;
    
    confess "Invalid event class $msg_class"
        unless $msg_class =~ /^[\w:\.]+$/sm;
    
    # find compiled event subclass
    $msg_class =~ s/\./::/g;
    $msg_class = "Grids::Message::$msg_class";

    # verify this class exists
    return unless eval { $msg_class->can('base') };
    
    return $msg_class;
}

1;
