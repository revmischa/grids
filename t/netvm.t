use strict;
use Test::More qw(no_plan);
use lib 'lib/perl';
use NetCode;
use NetVM;

# sample program
my $prog = slurp('sample/netvm_test.netasm');

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

$vm->step;
is($vm->pc, 0, 'j');

# convert to unsigned
sub _u { return int(sprintf("%u", $_[0])); }

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
