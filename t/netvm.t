use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetCode;
use NetVM;

# sample program
my $prog = q {
  beginning:
  li $t0, 0x12345678
  addi $t1, $t0, 1
  j beginning
};

my $bytecode = NetCode->assemble($prog);
ok($bytecode, "assembled program");

my $vm = new NetVM;
is($vm->pc, 0, "initted vm");
$vm->load($bytecode);

$vm->step;
is($vm->reg('t0'), 0x12345678, 'li');

$vm->step;
is($vm->reg('t1'), 0x12345679, 'addi');

$vm->step;
is($vm->pc, 0, 'j');
