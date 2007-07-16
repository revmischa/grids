use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetIdentity;

my $alice = NetIdentity->create(name => "Alice", verbose => 1, size => 512, passphrase => 'alicepass');
$alice->privkey->check;
my $id_ser = $alice->serialize;
$alice->decrypt_privkey('alicepass');

my $alice2 = NetIdentity->deserialize($id_ser);
$alice2->decrypt_privkey('alicepass');

my $bob = NetIdentity->create(name => "Bob", verbose => 1, size => 512);

my $plaintext = 'lol dongues';

# alice encrypt message for bob
{
    my $ciphertext = $alice->encrypt($plaintext, $bob);
    is($plaintext, $bob->decrypt($ciphertext), "public-key encrypted message");
}

# bob encrypt message for alice, make sure alice and alice2 can read it
{
    my $ciphertext = $bob->encrypt($plaintext, $alice);
    is($plaintext, $alice->decrypt($ciphertext), "public-key encrypted message");
    is($plaintext, $alice2->decrypt($ciphertext), "deserialization and private key decryption");
}

