# This package represents a Net Identity. All an identity needs is a
# private key, although other attributes may be associated with an
# identity.

use strict;

package NetIdentity;

use Crypt::RSA;
use Crypt::RSA::Key::Public;
use Crypt::RSA::Key::Private;

use Carp qw(croak);
use Data::Dumper;

sub new {
    my ($class, %opts) = @_;

    my $privkey = delete $opts{privkey};
    my $pubkey = delete $opts{pubkey};
    my $name = delete $opts{name};

    my $self = {
        pubkey  => $pubkey,
        privkey => $privkey,
        name    => $name,
    };

    bless $self, $class;
    return $self;
}

sub create {
    my ($class, %opts) = @_;

    my $verbose = delete $opts{verbose} ? 1 : 0;
    my $size = delete $opts{size} || 2048;
    my $passphrase = delete $opts{passphrase} || '';

    # generate a private key
    my $rsa = new Crypt::RSA;
    my ($pubkey, $privkey) = $rsa->keygen(
                                          Size   => $size,
                                          Cipher => 'Blowfish',
                                          Password => $passphrase,
                                          Verbosity => $verbose,
                                          );

    die "Error generating keypair: " . $rsa->errstr . "\n" unless $pubkey && $privkey;

    return $class->new(
                       pubkey => $pubkey,
                       privkey => $privkey,
                       %opts,
                       );
}

*pub = \&pubkey;
*priv = \&privkey;
sub privkey { $_[0]->{privkey} }

sub name { $_[0]->{name} }

sub serialize {
    my $self = shift;

    my $store = {
        name => $self->name,
        pubkey => $self->pub->serialize,
        privkey => $self->priv->serialize,
    };

    local $Data::Dumper::Purity = 1;         # fill in the holes for eval
    local $Data::Dumper::Deepcopy = 1;       # avoid cross-refs

    my $dump = Data::Dumper->Dump([$store], ['*store']); 
    return $dump;
}

sub deserialize {
    my ($class, $serialized) = @_;

    my %store;
    eval $serialized or die "Error unserializing: $@";

    my $pubkey = delete $store{pubkey};
    my $privkey = delete $store{privkey};

    $pubkey = Crypt::RSA::Key::Public->deserialize(String => [$pubkey]) if $pubkey;
    $privkey = Crypt::RSA::Key::Private->deserialize(String => [$privkey]) if $privkey;

    return $class->new(%store, pubkey => $pubkey, privkey => $privkey);
}

sub pubkey {
    my $self = shift;

    return $self->{pubkey} if $self->{pubkey};

    return $self->generate_pubkey;
}

sub generate_pubkey {
    my $self = shift;

    # TODO: generate pubkey
    croak "generate_pubkey not yet implemented";
}
