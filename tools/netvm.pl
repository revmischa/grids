#!/usr/bin/perl
use strict;
use lib 'lib/perl';
use NetVM;

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
    load(undef, $infile);
    $msg = dis_current_instruction();
}

my $con = NetConsole->new(
                          title    => "NetVM",
                          prompt   => "NetVM> ",
                          handlers => \%handlers,
                          message  => $msg,
                          );

$con->run;


########### commands

sub load {
    my ($self, $filename) = @_;

    return "File $file does not exist" unless -e $filename;
    my $program = NetCode::Program->load_from_file($filename)
        or return "Unable to load $filename";

    return vmload($program->segments);
}

sub asld {
    my ($self, $filename) = @_;
    my $code = slurp($filename);
    my $program = NetCode->assemble($code);
    return vmload($program->segments);
}
sub regs {
    my @args = @_;

    my $regs = $vm->regs;

    my $ret = '';
    my $i = 0;

    $ret .= sprintf "PC: 0x%08X\n", $vm->pc;

    my $regcount = @NetVM::REGS - 1;
    foreach my $reg (0 .. $regcount) {
        my $regname = $vm->reg_name($i);
        $ret .= sprintf "[%02d %4s]: 0x%08X   ", $i++, $regname, $vm->reg($reg);
        $ret .= "\n" if $i % 2 == 0;
    }

    return $ret;
}

sub reset {
    my $self = shift;
    $vm->init;
    return $self->load($infile);
}

sub run {
    my ($self) = @_;

    print "Running...\n";
    while ($vm->step) {

    }

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
    return sprintf "0x%08X> %s\n", $vm->pc, NetCode->disassemble_string($vm->current_instruction);
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
load (filename) - loads a file containing NetCode
asld (filename) - (assemble and load) assemble a NetAsm file and load it
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

    $vm->load($bytecode);

    printf "Loaded %i bytes\n", length $bytecode;

    return dis_current_instruction();
}

