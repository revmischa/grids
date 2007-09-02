use strict;
package NetClient;
use NetTransport;
use NetProtocol;
use Carp qw (croak);

use base qw/Class::Accessor/;
__PACKAGE__->mk_accessors(qw/transport id conf proto transport debug/);

# opts: id, transport, conf
# other opts passed to transport
sub new {
    my ($class, %opts) = @_;

    my $id          = delete $opts{id} or croak "No identity provided";
    my $trans_class = delete $opts{transport} || 'TCP';
    my $enc_class   = delete $opts{encapsulation} || 'JSON';

    my $proto = NetProtocol->new(
                                 encapsulation => $enc_class,
                                 event_handler => "$class::event_handler",
                                 );

    my $self = {
        conf => $opts{conf},
        debug => $opts{debug},
        id   => $id,
        proto => $proto,
    };

    bless $self, $class;

    my $t = "NetTransport::$trans_class"->new($self, %opts);
    $self->{transport} = $t;

    return $self;
}

# transmits a protocol request over the specified transport
sub do_request {
    my ($self, $method, $args) = @_;

    my $msg = $self->proto->encapsulate($method, $args);
    return 0 unless $msg;

    $self->transport->write($msg);
}

sub connection_established {
    my ($self, $trans, $con) = @_;

    $self->dbg("client transport $trans received connection: $con\n");
}

sub dbg {
    my ($self, $msg) = @_;
    return unless $self->debug;
    warn $msg;
}

1;
