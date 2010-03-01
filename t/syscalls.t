use Test::More qw(no_plan);

BEGIN {
    use FindBin;
    require "$FindBin::Bin/load_vm.pl";
};

use strict;
use warnings;

use Grids::Code;
use Grids::VM;
require "include/grids_posix.h";

use Data::Dumper;

# syscall test program
my $prog = slurp('sample/syscalls_test.gridsasm') or die "Couldn't load vm_test.gridsasm";

my $program = Grids::Code->assemble_program($prog)
    or die "Unable to assemble program";

my $segments = $program->segments;
ok(%$segments, "assembled program");

my $vm = new Grids::VM(show_warnings => 1, show_debug => 1);
$vm->load_syscall_module('Test');
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

$vm->show_warnings(0);
$vm->step; # syscall write
$vm->show_warnings(1);
is($vm->reg('v0'), -1, "got error writing to unopen fd");

$vm->step; # la
$vm->step; # la
$vm->step; # lw
$vm->step; # li
my $buf_len = $vm->reg_u('a2');
$vm->step; # read()
is($vm->reg('v0'), $buf_len, "got read() return value");

$vm->step; # lb
is($vm->reg('t0'), ord('A'), "read data into buffer");

