# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl GridsMem.t'

#########################

use Test::More qw(no_plan);
use strict;
use warnings;
BEGIN { use_ok('Grids::VM::Memory') };

#########################

# 16 bytes
my $mem = Grids::VM::Memory->new(16);
ok($mem, "got handle");
ok($mem->h, "mem handle is defined");

$mem->set(3, pack("C", 25));
my $val = $mem->get(3, 1);
is(unpack("C", $val), 25, "retreived byte");

$mem->set(5, pack("c*", 12, 34, 56, 78));
$val = $mem->get(5, 4);
my @vals = unpack("c*", $val);
is_deeply(\@vals, [12, 34, 56, 78], "retreived 4 bytes");

$mem->set(5, pack("C", 88));
$mem->resize(6);
is(unpack("C", $mem->get(5,1)), 88, "resize");

my $bytes = pack("C*", 0xa1, 0xb2, 0xc3, 0xd4, 0xe5, 0xf6);
$mem->load($bytes);
is(unpack("C", $mem->get(0, 1)), 0xa1, "load");
is(unpack("C", $mem->get(2, 1)), 0xc3, "load");
is(unpack("C", $mem->get($mem->size - 1, 1)), 0xf6, "load + size");
