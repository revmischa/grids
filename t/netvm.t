use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetCode;
use NetVM;
use Data::Dumper;

# sample program
my $prog = slurp('sample/netvm_test.netasm');

my $program = NetCode->assemble_program($prog)
    or die "Unable to assemble program";

my $segments = $program->segments;
ok(%$segments, "assembled program");

my $vm = new NetVM;
is($vm->pc, 0, "initted vm");
$vm->load_program($program);

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
is(chr($vm->reg('t0')), 'o', 'load memory byte');

$vm->step;
is(chr($vm->reg('t0')), '!', 'load memory byte');

$vm->step;
is($vm->reg('t1'), 0b00100000, 'andi');

$vm->step;
$vm->step;
is($vm->reg('t3'), 0b00000001, 'andi');

$vm->step;
$vm->step;
$vm->step;
is($vm->reg('t0'), 0b00100001, 'and');

my $pc = $vm->pc;
$vm->step;
is($vm->pc, $pc + 18, 'jreli');
is(op(), 'jreli', 'jrel op');

$pc = $vm->pc;
$vm->step; # jreli -2
is($vm->pc, $pc - 12, 'negative jreli');
is(op_r(), 'sll', 'negative jreli');

$vm->step; # nop
is(op(), 'j', 'nop');

$vm->step; # j testbranching
is(op(), 'addi', 'jump ok');

# test branching
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
$vm->step; # or $t1, $zero, 0xFFFFFFFF
is($vm->reg_u('t1'), 0xFFFFFFFF, 'or');
$vm->step; # li $t3, 1
is($vm->reg_u('t3'), 1, 'li');
$vm->step; # subu $t2, $t1, $t3
is($vm->reg_u('t2'), 0xFFFFFFFE, 'subu');

$vm->step; # j end
$vm->step; # j beginning
is($vm->pc, 0, 'j');


# convert to unsigned
sub _u { return int(sprintf("%u", $_[0])); }

# get opcode mnemonic
sub op { NetCode->opcode_mnemonic($vm->current_instruction_opcode); }

# get opcode mnemonic for r-type
sub op_r { NetCode->r_function_mnemonic($vm->current_instruction_r_func); }

# read in a file
sub slurp {
    my $filename = shift;

    unless (-e $filename) {
        print STDERR "$filename does not exist.\n";
        return undef;
    }

    my ($contents, $in);
    unless (open($in, $filename)) {
        print STDERR "Could not open $filename: $!\n";
        return undef;
    }

    # slurp in file
    do {
        local $/;
        $contents = <$in>;
    };

    close $in;

    return $contents;
}
