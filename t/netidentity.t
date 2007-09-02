use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetIdentity;

my $keysize = 512;

my $alice = NetIdentity->create(name => "Alice", verbose => 1, size => $keysize, passphrase => 'alicepass');
$alice->privkey->check;

my $id_ser = $alice->serialize;
ok($alice->decrypt_privkey('alicepass'), 'passphrase');

my $alice2 = NetIdentity->deserialize($id_ser);
ok($alice2->decrypt_privkey('alicepass'), 'passphrase');

#ok($alice->check, "key ok");
#ok($alice2->check, "key ok after serialization/deserialization/decryption");

my $bob = NetIdentity->create(name => "Bob", verbose => 1, size => $keysize);

my $tongds = $bob->serialize;
$bob->privkey->reveal;
ok($bob->check, 'serialize ok');
warn "serialized";
#my $bob2 = NetIdentity->deserialize($id_ser);
#warn $id_ser;
#is_deeply($bob2, $bob, "serialize/deserialize");

#ok($bob2->check, "key ok after deserialization");

my $plaintext = 'lol dongues';

# alice encrypt message for bob
{
    my $ciphertext = $alice->encrypt($plaintext, $bob);
    is($bob->decrypt($ciphertext), $plaintext, "public-key encrypted message");
}

# bob encrypt message for alice, make sure alice and alice2 can read it
{
    my $ciphertext = $bob->encrypt($plaintext, $alice);
    is($alice->decrypt($ciphertext), $plaintext, "public-key encrypted message");
    is($alice2->decrypt($ciphertext), $plaintext, "deserialization and private key decryption");
}

# sign and verify a message
{
    my $sig = $alice->sign($plaintext);
    ok(NetIdentity->verify($plaintext, $sig, $alice), "signed message with pubkey");
}
