use strict;
package NetClient;
use NetTransport;
use NetProtocol;
use Carp qw (croak);

use Class::Autouse(qw/NetProtocol::EventQueue/);

use base qw/Class::Accessor::Fast/;
__PACKAGE__->mk_accessors(qw/transport id conf proto transport debug session_token event_queue/);

# add hook support
do 'nethooks.pl' or die $@;

# opts: id, transport, conf
# other opts passed to transport
sub new {
    my ($class, %opts) = @_;

    my $id          = delete $opts{id} or croak "No identity provided";
    my $trans_class = delete $opts{transport} || 'TCP';
    my $enc_class   = delete $opts{encapsulation} || 'JSON';

    my $evt_queue   = NetProtocol::EventQueue->new;

    my $self = {
        event_queue => $evt_queue,
        conf  => $opts{conf},
        debug => $opts{debug},
        id    => $id,
        hooks => {},
    };

    bless $self, $class;

    my $proto = NetProtocol->new(encapsulation => $enc_class);
    $self->{proto} = $proto;

    my $t = "NetTransport::$trans_class"->new($self, %opts);
    $self->{transport} = $t;

    return $self;
}

# called when our transport receives data
sub data_received {
    my ($self, $trans, $data) = @_;

    $self->dbg("received data [$data]");

    my $evt = $self->proto->parse_request($data);
    $evt->{_trans} = $trans;
    $self->event_queue->add($evt) if $evt;
}

# processes everything in the event queue
sub flush_event_queue {
    my ($self) = @_;
    while ($self->do_next_event) {}
}

# fetches next event from event queue and handles it
# returns true if handled an event
sub do_next_event {
    my ($self) = @_;

    my $event = $self->event_queue->shift
        or return 0;

    $self->dbg("Handling event " . $event->event_name);

    my @hook_results = $self->run_event_hooks(event => $event->event_name,
                                              args => $event->args);

    # were there any results?
    if (@hook_results) {
        # if any hooks returned hashrefs of request arguments, do those requests
        foreach my $res (@hook_results) {
            next unless ref $res && ref $res eq 'HASH';

            # default the return request to be of the same method
            my $res_evt = $res->{event} || $event->event_name;
            $self->do_request($event->event_name, $res);
        }
    }

    return 1;
}

# transmits a protocol request over the specified transport
sub do_request {
    my ($self, $method, $argsref) = @_;

    # copy args so we don't modify anything
    my %args = $argsref ? %$argsref : ();

    if ($self->session_token) {
        $args{_session_token} = $self->session_token;
    }

    my $msg = $self->proto->encapsulate($method, \%args);
    return 0 unless $msg;

    $self->transport->write($msg);
}

# attempt to connect to a Node
sub connect {
    my ($self, $address, $node_public_key) = @_;

    $self->transport->connect($address);
}

# called when a connection with a Node has been established
sub connection_established {
    my ($self, $trans, $con) = @_;

    $self->dbg("client transport $trans received connection: $con\n");
    $self->transport->write($self->proto->initiation_string);
}

sub dbg {
    my ($self, $msg) = @_;
    return unless $self->debug;
    warn "NetClient: [Debug] $msg\n";
}

sub warn {
    my ($self, $msg) = @_;
    warn "NetClient: [Warn] $msg\n";
}

1;
