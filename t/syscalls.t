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

# syscall test program
my $prog = slurp('sample/syscalls_test.gridsasm') or die "Couldn't load vm_test.gridsasm";

my $program = Grids::Code->assemble_program($prog)
    or die "Unable to assemble program";

my $segments = $program->segments;
ok(%$segments, "assembled program");

my $vm = new Grids::VM(show_warnings => 0);
is($vm->pc, 0, "initted vm");
$vm->load_program($program);

$vm->step; # la
$vm->step; # li
$vm->step; # li
$vm->step; # syscall debug.logstr

$vm->step; # li
$vm->step; # la
$vm->step; # lw
my $teststr = "Hello, world!\n";
is($vm->reg('a2'), length($teststr), "assembling string length");
$vm->step; # li
$vm->step; # syscall write

# check return value
is($vm->reg('v0'), length($teststr), "got write() return value");

$vm->step; # li
$vm->step; # li
$vm->step; # syscall write
is($vm->reg('v0'), -1, "got error writing to unopen fd");
