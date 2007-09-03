# This is a class for a Node on the Net.  
# It handles sending/receiving data over transports and handling requests

use strict;
package NetNode;
use NetCode;
use NetVM;
use NetConf;
use NetTransport;
use Carp qw (croak);

use base qw/Class::Accessor::Fast/;
__PACKAGE__->mk_accessors(qw/conf transports proto sessions debug/);

our $default_conf = { };

# add hook support
do 'nethooks.pl' or die $@;

sub new {
    my ($class, %opts) = @_;

    my $conf = $opts{conf};
    my $debug = $opts{debug} || 0;

    # create default configuration if none specified
    unless ($conf) {
        $conf = NetConf->new;
    }

    my $self = {
        conf       => $conf,
        transports => [],
        debug      => $debug,
        sessions   => {},
    };

    bless $self, $class;

    return $self;
}

sub add_transport {
    my ($self, $trans_class, %opts) = @_;
    my $trans = "NetTransport::$trans_class"->new($self, %opts);
    push @{$self->{transports}}, $trans;
    return $trans;
}

sub connection_established {
    my ($self, $trans, $con) = @_;

    $self->dbg("server transport $trans received connection: $con\n");
}

sub data_received {
    my ($self, $trans, $data) = @_;

    $self->dbg("received data [$data]");

    if ($self->proto) {
        $self->proto->handle_request($data, $trans);
    } else {
        # if we don't have a protocol handler set up yet, this should be
        # the first transmission containing an initiation string
        $self->dbg("initating protocol handler with session init string [$data]");
        my $p = NetProtocol->new_from_initiation_string($data,
                                                        event_handler => \&handle_protocol_request,
                                                        event_handler_object => $self);

        unless ($p) {
            $self->warn("invalid initiation string [$data]");
            $trans->reset;
            return;
        }

        $self->{proto} = $p;

        $trans->write("==OK/" . $p->encap_base) or $self->dbg("Unable to write session init response");
        $self->session_initiated($trans);
    }
}

sub session_initiated {
    my ($self, $trans) = @_;

    $self->dbg("initiated session");
}

# protocol handler callback
sub handle_protocol_request {
    my ($self, $proto, $event, $args, $trans) = @_;

    $self->dbg("proto $proto got request $event");

    my @hook_results = $self->run_event_hooks(event => $event,
                                              args => $args,
                                              trans => $trans);

    # were there any results?
    if (@hook_results) {
        # if any hooks returned hashrefs of request arguments, do those requests
        foreach my $res (@hook_results) {
            next unless ref $res && ref $res eq 'HASH';

            # default the return request to be of the same method
            my $res_evt = $res->{event} || $event;
            $self->do_request($trans, $event, $res);
        }
    } else {
        return 0;
    }

    return 1;
}

sub do_request {
    my ($self, $trans, $event, $params) = @_;
    my $p = $self->proto;

    unless ($p) {
        warn "attempted to write data without initalized protocol handler";
        return 0;
    }

    my $serialized_data = $p->encapsulate($event, $params);
    return 0 unless $serialized_data;

    return $trans->write($serialized_data);
}

sub services {
    my ($self) = @_;

    return qw/Service1 Service2/;
}

sub check_authentication {
    my ($self, $req) = @_;

    my $session_token = delete $req->{args}{_session_token} or return 0;
    return $self->check_session_token($session_token);
}

sub check_session_token {
    my ($self, $token) = @_;

    my $remote = $self->sessions->{$token};

    return $remote;
}

sub authorized_keys {
    my ($self) = @_;

    my $pubkeys = $self->conf->get('Node.AuthorizedKeys') || {};
    return %$pubkeys;
}

sub dbg {
    my ($self, $msg) = @_;
    return unless $self->debug;
    warn "NetNode:   $msg\n";
}

sub warn {
    my ($self, $msg) = @_;
    warn "NetNode:   $msg\n";
}

1;
