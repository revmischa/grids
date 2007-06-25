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
  addi $t1, $t1, -4
  addiu $a0, $zero, 5
  addiu $a1, $a0, 2
  addi $a2, $a1, -1
  sll $at, $a2, 3
  srl $v0, $at, 1
  xor $a0, $a0, $a0
  addiu $a0, $a0, 0xFFFFFFFE
  xori $a0, $a0, 1
  li $v0, teststring
  lb $t0, 1($v0)
  lb $t0, 8($v0)
  j beginning

 teststring:
  .dz "cody sux!"
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
is($vm->reg('t1'), 0x12345675, 'addi subtract');

$vm->step;
is($vm->reg('a0'), 5, 'addiu');

$vm->step;
is($vm->reg('a1'), 7, 'addiu');

$vm->step;
is($vm->reg('a2'), 6, 'addi subtract');

$vm->step;
is($vm->reg('at'), 6 << 3, 'sll');

$vm->step;
is($vm->reg('v0'), $vm->reg('at') >> 1, 'srl');

$vm->step;
is($vm->reg('a0'), 0, 'xor');

$vm->step;
is($vm->reg('a0'), 0xFFFFFFFE, 'addiu');

$vm->step;
is($vm->reg('a0'), 0xFFFFFFFF, 'xori');

$vm->step;
$vm->step;
is(chr(unpack("CCCC", $vm->reg('t0'))), 'o', 'load memory byte');

$vm->step;
is(chr(unpack("CCCC", $vm->reg('t0'))), '!', 'load memory byte');

$vm->step;
is($vm->pc, 0, 'j');
