use strict;
package NetClient;
use NetTransport;
use NetProtocol;
use Carp qw (croak);

use base qw/Class::Accessor::Fast/;
__PACKAGE__->mk_accessors(qw/transport id conf proto transport debug/);

# add hook support
do 'nethooks.pl' or die $@;

# opts: id, transport, conf
# other opts passed to transport
sub new {
    my ($class, %opts) = @_;

    my $id          = delete $opts{id} or croak "No identity provided";
    my $trans_class = delete $opts{transport} || 'TCP';
    my $enc_class   = delete $opts{encapsulation} || 'JSON';

    my $self = {
        conf  => $opts{conf},
        debug => $opts{debug},
        id    => $id,
        hooks => {},
    };

    bless $self, $class;

    my $proto = NetProtocol->new(
                                 encapsulation => $enc_class,
                                 event_handler => \&event_handler,
                                 event_handler_object => $self,
                                 );
    $self->{proto} = $proto;

    my $t = "NetTransport::$trans_class"->new($self, %opts);
    $self->{transport} = $t;

    return $self;
}

# called when our transport receives data
sub data_received {
    my ($self, $trans, $data) = @_;

    $self->dbg("received data [$data]");

    $self->proto->handle_request($data);
}

# called on protocol events
sub event_handler {
    my ($self, $proto, $event, $args) = @_;

    $self->dbg("received event $event");

    my @hook_results = $self->run_event_hooks(event => $event,
                                              args => $args);

    # were there any results?
    if (@hook_results) {
        # if any hooks returned hashrefs of request arguments, do those requests
        foreach my $res (@hook_results) {
            next unless ref $res && ref $res eq 'HASH';

            # default the return request to be of the same method
            my $res_evt = $res->{event} || $event;
            $self->do_request($event, $res);
        }
    } else {
        return 0;
    }
}

# transmits a protocol request over the specified transport
sub do_request {
    my ($self, $method, $args) = @_;

    my $msg = $self->proto->encapsulate($method, $args);
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

    $self->transport->write($self->proto->initiation_string);
    $self->dbg("client transport $trans received connection: $con\n");
}

sub dbg {
    my ($self, $msg) = @_;
    return unless $self->debug;
    warn "NetClient: $msg\n";
}

1;
