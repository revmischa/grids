use Test::More qw(no_plan);

BEGIN {
    use FindBin;
    require "$FindBin::Bin/load_vm.pl";
};

use strict;
use warnings;

use Grids::Code;
use Grids::VM;
use Data::Dumper;

# sample program
my $prog = slurp('sample/vm_test.gridsasm') or die "Couldn't load vm_test.gridsasm";

my $program = Grids::Code->assemble_program($prog)
    or die "Unable to assemble program";

my $segments = $program->segments;
ok(%$segments, "assembled program");

my $vm = new Grids::VM;
is($vm->pc, 0, "initted vm");
$vm->load_program($program);

# test arithmetic
{
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
    is($vm->reg_u('a0'), 0xFFFFFFFE, 'addiu');

    $vm->step;
    is($vm->reg_u('a0'), 0xFFFFFFFF, 'xori');

    $vm->step;
    is($vm->reg('a1'), 0x7FFFFFFF, 'andi');
}

# test lb
{
    $vm->step;
    $vm->step;
    is(chr($vm->reg('t0')), 'o', 'load memory byte');

    $vm->step;
    is(chr($vm->reg('t0')), '!', 'load memory byte');
}

# test andi
{
    $vm->step;
    is($vm->reg('t1'), 0b00100000, 'andi');

    $vm->step;
    $vm->step;
    is($vm->reg('t3'), 0b00000001, 'andi');
}

# test multu
{
    $vm->step; # li
    $vm->step; # li
    $vm->step; # multu
    $vm->step; # mfhi
    $vm->step; # mflo
    is($vm->reg_u('t1'), 0x00ABCD12, "multu hi word");
    is($vm->reg_u('t2'), 0x33B7953A, "multu lo word");
    is($vm->reg_u('hi'), 0x00ABCD12, "multu hi word");
    is($vm->reg_u('lo'), 0x33B7953A, "multu lo word");
    is($vm->reg('hi'), $vm->reg('t1'), "mfhi");
    is($vm->reg_u('lo'), $vm->reg_u('t2'), "mflo");
}

# test div
{
    ;
}

# test and
{
    $vm->step;
    $vm->step;
    $vm->step;
    is($vm->reg('t0'), 0b00100001, 'and');
}

# test memory store/retrieve
{
    $vm->step; # la memtest1
    $vm->step; # li t1, 0x65
    is(op(), 'sb', 'sb op');
    $vm->step; # sb $t1, 2($t0)
    is(op(), 'addi', 'la op');
    $vm->step; # la memtest1step2
    is($vm->reg_u('t0') + 4, $vm->reg_u('t2'), "pointers line up");
    my $t0 = $vm->get_mem_u($vm->reg_u('t0'), 4);
    my $t2 = $vm->get_mem_u($vm->reg_u('t2'), 4);
    is($t0, $t2, "sb");

    is(op(), 'addi', 'li op');
    $vm->step; # li
    $vm->step; # sb
    is(pack('l', $vm->get_mem($vm->reg('t0'), 4)), "abef", "sb");

    is(op(), 'addi', 'addi');
    $vm->step; # la

    $vm->step; # lh
    $vm->step; # lh
    is($vm->reg_u('t1'), 0x00FF, "lh");
    is($vm->get_mem_u($vm->reg('t0') + 2, 2), 0x00FF, "get_mem_u 16");
    is($vm->get_mem_u($vm->reg('t0'), 4), 0x00FF1234, "get_mem_u 32");

    $vm->step; # addi

    is(op(), 'lh', 'lh op');
    $vm->step; # lh
    is($vm->reg_u('t2'), $vm->reg_u('t3'), "lh negative offset");
    $vm->step; # lh
    is($vm->reg('t2'), $vm->reg('t1'), "lh 0 offset");
}

# test unconditional branching
{
    my $pc = $vm->pc;
    $vm->step;
    is($vm->pc, $pc + 18, 'jreli');
    is(op(), 'jreli', 'jrel op');

    $pc = $vm->pc;
    $vm->step; # jreli -2
    is($vm->pc, $pc - 12, 'negative jreli');
    is(op_r(), 'sll', 'negative jreli');
}

# nop
$vm->step;
is(op(), 'j', 'nop');

# jump
$vm->step; # j testbranching
is(op(), 'addi', 'jump ok');

# test conditional branching
{

    $vm->step; # li
    is($vm->reg('t1'), 0xf3d8, 'li');
    $vm->step; # li
    is($vm->reg('t2'), 0xf3d8, 'li');
    $vm->step; # beq
    is(op(), 'j', 'beq');

    $vm->step; # j t2
    is(op(), 'addi', 'beq');

    $vm->step; # li
    is(op(), 'bne', 'beq');
    $vm->step; # bne $t1, $t2, good2
    is(op(), 'j', 'bne');

    $vm->step; # j

    # t3
    is(op(), 'addi', 'bne');
    $vm->step; # li
    $vm->step; # bgez
    is(op_r(), 'sll', 'bgez');
    $vm->step; # nop

    is(op(), 'bgez', 'bgez');
    $vm->step; # bgez

    my $curpc = $vm->pc;
    is(op(), 'bgezal', 'bgez');

    $vm->step; # bgezal
    is($vm->reg('ra'), $curpc + 6, 'bgezal set $ra');
    is($vm->pc, $curpc + 12, 'bgezal branch');

    is(op_r(), 'jr', 'bgezal');

    $vm->step; # jr $ra
    is(op(), 'j', 'jr $ra');

    # t6
    $vm->step; # j t6
    $vm->step; # xori
    is($vm->reg('t0'), 0, 'xor');
    $vm->step; # bltz $t0, end
    is(op(), 'addi', 'bltz');

    # t7
    $vm->step; # addi $t0, $t0, -1
    is($vm->reg('t0'), -1, 'addi subtract');
    $vm->step; # bltz $t0, t8
    is(op(), 'addi', 'bltz');

    # t8
    $vm->step; # addi $t0, $t0, 1
    is($vm->reg('t0'), 0, 'addi');
    $vm->step; # blez $t0, br_done
    is(op(), 'jal', 'blez');

    #### end of branch tests
}

$vm->step; # jal testjal
is(op_r(), 'jr', 'jal');
$vm->step; # jr $ra
is(op_r(), 'sll', 'jr $ra');

$vm->step; # nop
$vm->step; # li $t4, 0xFFFFFFFF

# test subu
{
    $vm->step; # or $t1, $zero, 0xFFFFFFFF
    is($vm->reg_u('t1'), 0xFFFFFFFF, 'or');
    $vm->step; # li $t3, 1
    is($vm->reg_u('t3'), 1, 'li');
    $vm->step; # subu $t2, $t1, $t3
    is($vm->reg_u('t2'), 0xFFFFFFFE, 'subu');
}

$vm->step; # j end
$vm->step; # j beginning
is($vm->pc, 0, 'j');

# get opcode mnemonic
sub op { Grids::Code->opcode_mnemonic($vm->current_instruction_opcode); }

# get opcode mnemonic for r-type
sub op_r { Grids::Code->r_function_mnemonic($vm->current_instruction_r_func); }
