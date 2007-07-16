use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetIdentity;

my $id = NetIdentity->create(name => "Colonel McCheese", verbose => 1, size => 48);
my $id_ser = $id->serialize;
is_deeply($id, NetIdentity->deserialize($id_ser), "serializing/deserializing");



