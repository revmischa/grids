#!/usr/bin/perl
use strict;

use FindBin;
use lib "$FindBin::Bin/../lib";

use lib "$FindBin::Bin/../lib/Grids/VM/Memory/lib";
use lib "$FindBin::Bin/../lib/Grids/VM/Memory/blib/arch/auto/Grids/VM/Memory";
use lib "$FindBin::Bin/../lib/Grids/VM/Register/lib";
use lib "$FindBin::Bin/../lib/Grids/VM/Register/blib/arch/auto/Grids/VM/Register";

use Grids::VM;
use Grids::Console;
use Grids::Code::Program;

use AnyEvent;

my $infile = shift;

my $vm = Grids::VM->new(memory_limit => 16 * 1024 * 1024);

my %handlers = (
                help  => \&help,
                load  => \&load,
                asld  => \&asld,
                step  => \&step,
                st    => \&step,
                s     => \&step,
                mem   => \&mem,
                run   => \&run,
                r     => \&run,
                regs  => \&regs,
                reg   => \&regs,
                reset => \&reset,
                );

my $msg;

if ($infile) {
    load(undef, $infile);
    $msg = dis_current_instruction();
}

my $main = AnyEvent->condvar;

my $con = Grids::Console->new(
    cv => $main,
    title    => "GridsVM",
    prompt   => "GridsVM> ",
    handlers => \%handlers,
    message  => $msg,
);

$con->listen_for_input;
$main->recv;


########### commands

sub load {
    my ($self, $filename) = @_;

    return "File $filename does not exist" unless -e $filename;
    my $program = Grids::Code::Program->load_from_file($filename)
        or return "Unable to load $filename";

    $vm->load_program($program);

    return "Loaded program";
}

sub asld {
    my ($self, $filename) = @_;
    my $code = slurp($filename);
    my $program = Grids::Code->assemble_program($code);
    return vmload($program->segments);
}
sub regs {
    my @args = @_;

    my $regs = $vm->regs;

    my $ret = '';
    my $i = 0;

    $ret .= sprintf "PC: 0x%08X\n", $vm->pc;

    my $regcount = @Grids::VM::REGS - 1;
    foreach my $reg (0 .. $regcount) {
        my $regname = $vm->reg_name($i);
        $ret .= sprintf "[%02d %4s]: 0x%08X   ", $i++, $regname, $vm->reg($reg);
        $ret .= "\n" if $i % 2 == 0;
    }

    return $ret;
}

sub reset {
    my $self = shift;
    
    $vm->init_regs;
    dis_current_instruction();
}

sub run {
    my ($self) = @_;

    print "Running...\n";
    $vm->run;
    return "Program reached end of memory";
}

sub step {
    my ($self) = @_;

    if ($vm->step) {
        return dis_current_instruction();
    } else {
        return "End of program reached";
    }
}

# returns disassembly of current instruction
sub dis_current_instruction {
    return sprintf "0x%08X> %s\n", $vm->pc, Grids::Code->disassemble_string($vm->current_instruction);
}

sub mem {
    my ($self, $len) = @_;

    $len ||= 256;

    my $ret = '';
    my $bytes;

    my $size = $vm->mem->size;
    
    $size = $size > $len ? $len : $size;
    for (my $pos = 0; $pos < $size; $pos++) {
        my $byte = unpack("C", $vm->mem->get($pos, 1));
        $ret .= sprintf "%08X: 0x%02X %3d 0b%08b %c\n", $pos, $byte, $byte, $byte, $byte;
    }

    return $ret;
}

sub help {
    my ($self) = @_;

    return q {
load (filename) - loads a file containing GridsCode
asld (filename) - (assemble and load) assemble a GridsAsm file and load it
mem - dump memory
step - step program one instruction
run - runs program
regs - dump registers
quit - quit
};
}

###########


# slurp in a file and return its contents, returns undef on error
sub slurp {
    my ($filename) = @_;

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

sub vmload {
    my $segment_map = shift;

    my $prog = new Grids::Code::Program(segments => $segment_map);
    $vm->load_program($prog);

    printf "Loaded %i bytes\n", length $prog->bytes;

    return dis_current_instruction();
}

