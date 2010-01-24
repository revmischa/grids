#!/usr/bin/perl

use warnings;
use strict;

use Test::More qw(no_plan);
use lib 'lib';
use Grids::Identity;

my $verbose = 0;

my $alice = Grids::Identity->create_for_test(name => "Alice", verbose => $verbose, passphrase => 'alicepass');

ok(-e $alice->otr->config_dir, "Created OTR config dir");

# force private key to be generated
$alice->otr->load_privkey;

ok(-e $alice->otr->keyfile, "Created OTR private key file");

#my $id_ser = $alice->serialize;
#ok($alice->decrypt_privkey('alicepass'), 'passphrase');

#my $alice2 = Grids::Identity->deserialize($id_ser);
#ok($alice2->decrypt_privkey('alicepass'), 'passphrase');

#ok($alice->check, "key ok");
#ok($alice2->check, "key ok after serialization/deserialization/decryption");

my $bob = Grids::Identity->create_for_test(name => "Bob", verbose => $verbose);

#my $tongds = $bob->serialize;
#ok($bob->check, 'no serialization side-effects');

my $plaintext = 'lol dongues';

# alice encrypt message for bob
if (0){
    my $ciphertext = $alice->encrypt($plaintext, $bob);
    is($bob->decrypt($ciphertext), $plaintext, "public-key encrypted message");
}

# bob encrypt message for alice, make sure alice and alice2 can read it
if (0){
    my $ciphertext = $bob->encrypt($plaintext, $alice);
    is($alice->decrypt($ciphertext), $plaintext, "public-key encrypted message");
#    is($alice2->decrypt($ciphertext), $plaintext, "deserialization and private key decryption");
}

# sign and verify a message
{
    my $sig = $alice->sign(\$plaintext);
    ok($sig, "signed message");
    ok(Grids::Identity->verify(\$plaintext, $sig, $alice->pubkey), "verified signed message with pubkey");
    ok(! Grids::Identity->verify(\"x$plaintext", $sig, $alice->pubkey), "message verification failure");
}
