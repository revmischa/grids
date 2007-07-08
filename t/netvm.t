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
  andi $a1, $a0, 0x7FFFFFFF
  li $v0, teststring
  lb $t0, 1($v0)
  lb $t0, 8($v0)
  andi $t1, $t0, 0b00111110
  li $t2, 0xFFFFFFFF
  andi $t3, $t2, 0b00000001

  li $t2, 0b10110101
  li $t1, 0b00101001
  and $t0, $t2, $t1

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
is(_u($vm->reg('a0')), 0xFFFFFFFE, 'addiu');

$vm->step;
is(_u($vm->reg('a0')), 0xFFFFFFFF, 'xori');

 $vm->step;
is($vm->reg('a1'), 0x7FFFFFFF, 'andi');

$vm->step;
$vm->step;
warn "t0: " . $vm->reg('t0');
is(chr($vm->reg('t0')), 'o', 'load memory byte');

$vm->step;
is(chr(unpack("CCCC", $vm->reg('t0'))), '!', 'load memory byte');

$vm->step;
is($vm->reg('t1'), 0b00100000, 'andi');

$vm->step;
$vm->step;
is($vm->reg('t3'), 0b00000001, 'andi');

$vm->step;
$vm->step;
$vm->step;
is($vm->reg('t0'), 0b00100001, 'and');

$vm->step;
is($vm->pc, 0, 'j');

# convert to unsigned
sub _u { return int(sprintf("%u", $_[0])); }
