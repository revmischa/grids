# This package represents a Grids Identity. All an identity needs is a
# public or private key, although other attributes may be associated
# with an identity.

use strict;

package Grids::Identity;

use Crypt::RSA;
use Grids::Identity::Public;
use Grids::Identity::Private;
use Crypt::RSA::ES::OAEP;
use Crypt::Blowfish;

use Carp qw(croak);
use Data::Dumper;

sub new {
    my ($class, %opts) = @_;

    my $privkey = delete $opts{privkey};
    my $pubkey = delete $opts{pubkey};
    my $name = delete $opts{name};
    my $encrypted = delete $opts{encrypted};

    my $self = {
        pubkey    => $pubkey,
        privkey   => $privkey,
        name      => $name,
        encrypted => $encrypted,
    };

    bless $self, $class;
    return $self;
}

sub create_for_test {
    my ($class, %opts) = @_;

    $opts{size} ||= 512;
    return $class->create(%opts);
}

# generate a public/private keypair
sub create {
    my ($class, %opts) = @_;

    my $verbose = delete $opts{verbose} ? 1 : 0;
    my $passphrase = delete $opts{passphrase} || '';

    my $size = delete $opts{size};
    $size ||= 2048;

    print "\nGenerating public/private keypair, this may take a little while...\n\n" if $verbose;

    # generate a private key
    my $rsa = new Crypt::RSA;
    my ($pubkey, $privkey) = $rsa->keygen(
                                          KF        => 'SSH',
                                          Size      => $size,
                                          Cipher    => 'Blowfish',
                                          Password  => $passphrase,
                                          Verbosity => $verbose,
                                          );

    die "Error generating keypair: " . $rsa->errstr . "\n" unless $pubkey && $privkey;

    print "\nGenerated identity keypair.\n\n" if $verbose;

    my $id = $class->new(
                         pubkey => $pubkey,
                         privkey => $privkey,
                         %opts,
                         );

    # save an encrypted string as part of our identity. this can be used to check if decrypting the key was successful
    my $encrypted = $passphrase ? $id->encrypt("GRIDS", $id) : 0;
    $id->{encrypted} = $encrypted;

    return $id;
}

# encrypt plaintext for $id
sub encrypt {
    my ($self, $plaintext, $id, %opts) = @_;
    my $rsa = new Crypt::RSA;
    return $rsa->encrypt(Message => $plaintext, Key => $id->pubkey, Armor => $opts{armor})
        or die $rsa->errstr;
}

# decrypt ciphertext for $id
sub decrypt {
    my ($id, $ciphertext, %opts) = @_;
    my $rsa = new Crypt::RSA;

    return $rsa->decrypt(Cyphertext => $ciphertext, Key => $id->privkey, Armor => $opts{armor})
        or die $rsa->errstr;
}

# sign plaintext with pubkey, return signature
sub sign {
    my ($id, $message, %opts) = @_;
    my $rsa = new Crypt::RSA;
    return $rsa->sign(Message => $message,
                      Key     => $id->privkey,
                      Armor   => $opts{armor}) or die $rsa->errstr;
}

# return if $message is signed by $id's pubkey
sub verify {
    my ($class, $message, $sig, $id, %opts) = @_;

    my $rsa = new Crypt::RSA;

    return $rsa->verify(Message   => $message,
                        Key       => $id->pubkey,
                        Signature => $sig,
                        Armor     => $opts{armor}) or die $rsa->errstr;
}

# decrypts private key with $passphrase, returns if successful
sub decrypt_privkey {
    my ($self, $passphrase) = @_;

    $self->privkey->reveal(Password => $passphrase);

    my $match = $self->keys_match;
    return $match;
}

# returns true if the stored pubkey is correct for the privkey
sub keys_match {
    my $self = shift;

    # we should be able to decrypt a message encrypted for our pubkey with our private key
    # 'GRIDS' is stored encrypted for our pubkey in $self->{encrypted}
    my $res = $self->decrypt($self->encrypted);
    return $res && $res eq 'GRIDS';

    my $rsa = new Crypt::RSA;

    my $ciphertext = $rsa->encrypt(Message => 'plaintext', Key => $self->pubkey)
        or die $rsa->errstr;

    my $plaintext = $rsa->decrypt(Cyphertext => $ciphertext, Key => $self->privkey)
        or return 0;

    return $plaintext eq 'plaintext';
}

# returns true if all keys check out ok
sub check {
    my $self = shift;
    my $ok = 1;

    $ok &&= $self->pubkey->check if $self->pubkey;
    $ok &&= $self->privkey->check if $self->privkey;

    return $ok;
}

sub encrypted { $_[0]->{encrypted} }

*pub = \&pubkey;
*priv = \&privkey;
sub privkey { $_[0]->{privkey} }

sub name { $_[0]->{name} }

# encrypt privkey
sub hide {
    my $self = shift;

    $self->priv->hide if $self->encrypted && $self->priv;
}

sub serialize {
    my $self = shift;    

    my $priv = $self->priv;
    my $pub = $self->pub;

    $self->hide;

    my $store = {
        name => $self->name,
        encrypted => $self->encrypted,
    };

    $store->{pubkey}  = $pub->serialize if $pub;
    $store->{privkey} = $priv->serialize if $priv;

    local $Data::Dumper::Purity = 1;         # fill in the holes for eval
    local $Data::Dumper::Deepcopy = 1;       # avoid cross-refs

    my $ser = Data::Dumper->Dump([$store], ['*store']); 


    # Crypt::RSA is super annoying and will actually serialize the
    # keys in-place. this means if you try to use the keys after
    # calling this method they won't work, they have to be
    # deserialized again.

    $self->{pubkey} = $pub->deserialize(String => [$store->{pubkey}]) if $pub;
    $self->{privkey} = $priv->deserialize(String => [$store->{privkey}]) if $priv;

    return $ser;
}

sub new_from_serialized_pubkey {
    my ($class, $pubkey_serialized) = @_;

    my $pubkey = Grids::Identity::Public->deserialize(String => [$pubkey_serialized]);
    return $class->new(pubkey => $pubkey);
}

sub deserialize {
    my ($class, $serialized) = @_;

    my %store;
    eval $serialized or die "Error unserializing: $@";

    my $pubkey = delete $store{pubkey};
    my $privkey = delete $store{privkey};

    $pubkey = Grids::Identity::Public->deserialize(String => [$pubkey]) if $pubkey;
    $privkey = Grids::Identity::Private->deserialize(String => [$privkey]) if $privkey;

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
