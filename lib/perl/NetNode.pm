# This is a class for a Node on the Net. You should not instantiate
# this base class but rather a subclass with a network transport
# defined.

use strict;
package NetNode;
use NetCode;
use NetVM;
use NetConf;
use NetTransport;
use Carp qw (croak);

our $default_conf = { };

sub new {
    my ($class, %opts) = @_;

    my $conf = $opts{conf} or croak "No conf";

    my $self = {
        conf       => $conf,
        transports => [],
    };

    bless $self, $class;

    return $self;
}

sub conf { $_[0]->{conf} }

sub add_transport {
    my ($self, $trans_class, %opts) = @_;
    my $trans = "NetTransport::$trans_class"->new($self, %opts);
    push @{$self->{transports}}, $trans;
    return $trans;
}

sub connection_established {
    my ($self, $trans, $con) = @_;

    print "received connection: $con\n";
}


sub services {
    my ($self) = @_;

}

1;
