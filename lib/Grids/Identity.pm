# This package represents a Grids Identity. All an identity needs is a
# public or private key, although other attributes may be associated
# with an identity.

package Grids::Identity;

use Moose;

use Grids::Identity::Public;
use Grids::Identity::Private;
use Grids::UUID;
use Crypt::OTR;

use Digest::SHA1;
use FindBin;
use Carp qw(croak);
use Data::Dumper;

# attributes
has 'conf_dir' => (
    is => 'rw',
);
has '_privkey' => (
    is => 'rw',
);
has '_pubkey' => (
    is => 'rw',
);
has 'encrypted' => (
    is => 'rw',
);
has 'name' => (
    is => 'rw',
    isa => 'Str',
    default => \&name_default,
    lazy => 1,
);
has 'otr' => (
    is => 'rw',
    lazy => 1,
    default => \&otr_default,
);

use constant {
#    MAX_MESSAGE_SIZE => 16350,
    MAX_MESSAGE_SIZE => 512,
    PROTOCOL_NAME    => 'Grids',
};


Crypt::OTR->init;

# fix this to be the public key
sub name_default {
    my ($self) = @_;

    return Grids::UUID->new_id;
}

# crypt::otr object
sub otr_default {
    my ($self) = @_;

    return Crypt::OTR->new(
        config_dir       => $self->conf_dir,
        account_name     => $self->name,
        max_message_size => MAX_MESSAGE_SIZE,
        protocol         => PROTOCOL_NAME,
    );
}

sub create_for_test {
    my ($class, %opts) = @_;

    $opts{conf_dir} ||= "$FindBin::Bin";

    return $class->new(%opts);
}

# load or generate private key (may block for a long time)
sub load_privkey {
    my ($self) = @_;
    $self->otr->load_privkey;
}    

# return a digest of $txt
sub digest {
    my ($self, $txt) = @_;

    return Digest::SHA1::sha1_hex($txt);
}

# sign plaintext with pubkey, return signature
sub sign {
    my ($id, $messageref, %opts) = @_;

    return $id->otr->sign($id->digest($$messageref));

    #my $rsa = new Crypt::RSA;
    #return $rsa->sign(Message => $message,
    #                  Key     => $id->privkey,
    #                  Armor   => $opts{armor}) or die $rsa->errstr;
}

# return if $message_ref is signed by $pubkey
sub verify {
    my ($class, $message_ref, $sig, $pubkey, %opts) = @_;

    return Crypt::OTR->verify($class->digest($$message_ref), $sig, $pubkey);

#    my $rsa = new Crypt::RSA;
#    return $rsa->verify(Message   => $message,
#                        Key       => $id->pubkey,
#                        Signature => $sig,
#                        Armor     => $opts{armor}) or die $rsa->errstr;
}

sub privkey {
    my $self = shift;
    return $self->_privkey;
}

sub pubkey {
    my $self = shift;

    return $self->_pubkey if $self->_pubkey;

    my $pk = $self->otr->pubkey;
    $self->_pubkey($pk);

    return $pk;
}

sub set_callback {
    my ($self, $event, $callback) = @_;

    $self->otr->set_callback($event, $callback);
}

# returns message encrypted for $recipient if we have their pubkey,
# otherwise returns undef
sub encrypt {
    my ($self, $recipient, $plaintext) = @_;
    return $self->otr->encrypt($recipient, $plaintext);
}

# returns message from $sender decrypted if possible
# otherwise returns undef
sub decrypt {
    my ($self, $sender, $ciphertext) = @_;
    return $self->otr->decrypt($sender, $ciphertext);
}

# start socialist millionaires protocol
# (http://en.wikipedia.org/wiki/Socialist_millionaire)
sub initiate_smp {
    my ($self, $recipient, $secret, $question) = @_;
    return $self->otr->start_smp($recipient, $secret, $question);
}

# respond to a SMP request
sub continue_smp {
    my ($self, $recipient, $secret) = @_;
    return $self->otr->continue_smp($recipient, $secret);
}

#####################
# NEED TO BE UPDATED:


# encrypt plaintext for $id
sub _encrypt {
    my ($self, $plaintext, $id, %opts) = @_;
    croak "don't call this";
    my $rsa = new Crypt::RSA;
    return $rsa->encrypt(Message => $plaintext, Key => $id->pubkey, Armor => $opts{armor})
        or die $rsa->errstr;
}

# decrypt ciphertext for $id
sub _decrypt {
    my ($id, $ciphertext, %opts) = @_;
    croak "don't call this";

    my $rsa = new Crypt::RSA;

    return $rsa->decrypt(Cyphertext => $ciphertext, Key => $id->privkey, Armor => $opts{armor})
        or die $rsa->errstr;
}

# generate a public/private keypair
sub create {
    my ($class, %opts) = @_;

    croak "don't call this";

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
                         _pubkey => $pubkey,
                         _privkey => $privkey,
                         %opts,
                         );

    # save an encrypted string as part of our identity. this can be used to check if decrypting the key was successful
    $id->privkey->reveal(Password => $passphrase) if $passphrase;
    my $encrypted = $passphrase ? $id->encrypt("GRIDS", $id) : 0;
    $id->{encrypted} = $encrypted;

    return $id;
}


# decrypts private key with $passphrase, returns if successful
sub decrypt_privkey {
    my ($self, $passphrase) = @_;

    croak "don't call this";
    $self->privkey->reveal(Password => $passphrase);

    my $match = $self->keys_match;
    return $match;
}

# returns true if the stored pubkey is correct for the privkey
sub keys_match {
    my $self = shift;

    croak "don't call this";

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
    croak "don't call this";
    my $self = shift;
    my $ok = 1;

    $ok &&= $self->pubkey->check if $self->pubkey;
    $ok &&= $self->privkey->check if $self->privkey;

    return $ok;
}

*pub = \&pubkey;
*priv = \&privkey;

# encrypt privkey
sub hide {
    my $self = shift;
    croak "don't call this";

    $self->priv->hide if $self->encrypted && $self->priv;
}

*stringify = \&serialize;
sub serialize {
    my $self = shift;    

    croak "don't call this";

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

    $self->_pubkey($pub->deserialize(String => [$store->{pubkey}])) if $pub;
    $self->_privkey($priv->deserialize(String => [$store->{privkey}])) if $priv;

    return $ser;
}

sub new_from_serialized_pubkey {
    my ($class, $pubkey_serialized) = @_;

    croak "don't call this";

    my $pubkey = Grids::Identity::Public->deserialize(String => [$pubkey_serialized]);
    return $class->new(_pubkey => $pubkey);
}

sub deserialize {
    my ($class, $serialized) = @_;

    croak "don't call this";

    my %store;
    eval $serialized or die "Error unserializing: $@";

    my $pubkey = delete $store{pubkey};
    my $privkey = delete $store{privkey};

    $pubkey = Grids::Identity::Public->deserialize(String => [$pubkey]) if $pubkey;
    $privkey = Grids::Identity::Private->deserialize(String => [$privkey]) if $privkey;

    return $class->new(%store, _pubkey => $pubkey, _privkey => $privkey);
}


1;
