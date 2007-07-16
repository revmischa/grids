use strict;
package NetClient;
use NetTransport;
use Carp qw (croak);

# opts: id, transport, conf
# other opts passed to transport
sub new {
    my ($class, %opts) = @_;

    my $id          = delete $opts{id} or croak "No identity provided";
    my $trans_class = delete $opts{transport} || 'TCP';
   
    my $self = {
        conf => $opts{conf},
        id   => $id,
    };

    bless $self, $class;

    my $t = "NetTransport::$trans_class"->new($self, %opts);
    $self->{transport} = $t;

    return $self;
}

*trans = \&transport;
sub transport { $_[0]->{transport} }
sub id { $_[0]->{id} };
sub conf { $_[0]->{conf} };
