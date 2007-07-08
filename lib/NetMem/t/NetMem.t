# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl NetMem.t'

#########################

use Test::More qw(no_plan);
BEGIN { use_ok('NetMem') };

#########################

# 16 bytes
my $mem = NetMem->new(16);
ok($mem, "got handle");
ok($mem->h, "mem handle is defined");

$mem->set(3, pack("C", 25));
my $val = $mem->get(3, 1);
is(unpack("C", $val), 25, "retreived byte");

$mem->set(5, pack("c*", 12, 34, 56, 78));
$val = $mem->get(5, 4);
my @vals = unpack("c*", $val);
is_deeply(\@vals, [12, 34, 56, 78], "retreived 4 bytes");
