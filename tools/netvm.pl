#!/usr/bin/perl
use strict;
use lib 'lib/perl';
use NetVM;
use MemHandle;
use IO::Seekable;

use NetConsole;

my $infile = shift;

my $vm = NetVM->new(memory_limit => 16);

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
    load($infile);
    $msg = dis_current_instruction();
}

my $con = NetConsole->new(
                          title    => "NetVM",
                          prompt   => "NetVM> ",
                          handlers => \%handlers,
                          message  => $msg,
                          );

$con->run;

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

sub load {
    my ($filename) = @_;

    my $contents = slurp($filename) or return undef;
    my $bytecode = pack("C*", unpack("C*", $contents));

    return vmload($bytecode);
}

sub asld {
    my $code = slurp(shift());
    my $bytecode = NetCode->assemble($code);
    return vmload($bytecode);
}

sub vmload {
    my $bytecode = shift;

    $vm->load($bytecode);

    printf "Loaded %i bytes\n", length $bytecode;

    return dis_current_instruction();
}

sub regs {
    my @args = @_;

    my $regs = $vm->regs;

    my $ret = '';
    my $i = 0;

    $ret .= sprintf "PC: 0x%08X\n", $vm->pc;

    foreach my $reg (@$regs) {
        my $regname = $vm->reg_name($i);
        $ret .= sprintf "[%02d %4s]: 0x%08X   ", $i++, $regname, $reg;
        $ret .= "\n" if $i % 2 == 0;
    }

    return $ret;
}

sub reset {
    $vm->init;
    return load($infile);
}

sub run {
    my @args = @_;

    print "Running...\n";
    while ($vm->step) {

    }

    return "Program reached end of memory";
}

sub step {
    my @args = @_;

    if ($vm->step) {
        return dis_current_instruction();
    } else {
        return "End of program reached";
    }
}

# returns disassembly of current instruction
sub dis_current_instruction {
    return sprintf "0x%08X> %s\n", $vm->pc, NetCode->disassemble_string($vm->current_instruction);
}

sub mem {
    my @args = @_;

    my $ret = '';
    my $bytes;
    $vm->mem->seek(0, SEEK_SET);
    my $pos = 0;
    while ($vm->mem->read($bytes, 128)) {
        foreach my $byte (unpack("C*", $bytes)) {
            $ret .= sprintf "%08X: 0x%04X 0b%08b %c\n", $pos, $byte, $byte, $byte;
            $pos++;
        }
        last if $pos > 15;
    }

    return $ret;
}

sub help {
    my @args = @_;

    return q {
load (filename) - loads a file containing NetCode
asld (filename) - (assemble and load) assemble a NetAsm file and load it
mem - dump memory
step - step program one instruction
run - runs program
regs - dump registers
quit - quit
};
}
