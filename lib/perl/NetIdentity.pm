# This package represents a Net Identity. All an identity needs is a
# public or private key, although other attributes may be associated
# with an identity.

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

# generate a public/private keypair
sub create {
    my ($class, %opts) = @_;

    my $verbose = delete $opts{verbose} ? 1 : 0;
    my $size = delete $opts{size} || 512;
    my $passphrase = delete $opts{passphrase} || '';

    print "\nGenerating public/private keypair, this may take a little while...\n\n" if $verbose;

    # generate a private key
    my $rsa = new Crypt::RSA;
    my ($pubkey, $privkey) = $rsa->keygen(
                                          Size   => $size,
                                          Cipher => 'Blowfish',
                                          Password => $passphrase,
                                          Verbosity => $verbose,
                                          );

    die "Error generating keypair: " . $rsa->errstr . "\n" unless $pubkey && $privkey;

    print "\nGenerated identity keypair.\n\n" if $verbose;

    return $class->new(
                       encrypted => $passphrase ? 1 : 0,
                       pubkey => $pubkey,
                       privkey => $privkey,
                       %opts,
                       );
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

    return $self->keys_match;
}

# returns true if the stored pubkey is correct for the privkey
sub keys_match {
    my $self = shift;

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

# warning! this will have a side-effect of encrypting the private key!
sub serialize {
    my $self = shift;

    warn "encrypted" if $self->encrypted;
    $self->priv->hide if $self->encrypted && $self->priv;

    my $store = {
        name => $self->name,
        encrypted => $self->encrypted,
    };

    $store->{pubkey} = $self->pub->serialize if $self->pub;
    $store->{privkey} = $self->priv->serialize if $self->priv;

    local $Data::Dumper::Purity = 1;         # fill in the holes for eval
    local $Data::Dumper::Deepcopy = 1;       # avoid cross-refs

    my $ser = Data::Dumper->Dump([$store], ['*store']); 
    return $ser;
}

sub deserialize {
    my ($class, $serialized) = @_;

    my %store;
    eval $serialized or die "Error unserializing: $@";

    my $pubkey = delete $store{pubkey};
    my $privkey = delete $store{privkey};

    warn "privkey: $privkey";

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
