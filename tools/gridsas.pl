#!/usr/bin/perl

# This is a simple frontend to the GridsCode assembler and disassembler
# run with -d to produce disassembly

use strict;

use FindBin;
use lib "$FindBin::Bin/../lib";

use Grids::Code;
use Getopt::Long;

my ($outfile, $dis);

GetOptions("o|output=s"    => \$outfile,
           "d|disassemble" => \$dis);

my $infile = shift;

usage() unless $infile;

die "$infile does not exist\n" unless -e $infile;

# read file in and assemble
{
    my ($in, $out, $contents);

    open($in, $infile) or die "Could not open $infile for reading: $!\n";
    local $/; # slurrrpp
    $contents = <$in>;
    close $in;

    my $code;

    if ($dis) {
        for (my $i = 0; $i < length $contents; $i+=6) {
            my $inst = substr($contents, $i, 6);
            my $addr = sprintf("%08X: ", $i);
            $code .= $addr . Grids::Code->disassemble_string($inst) . "\n";
        }
    } else {
        my $prog = Grids::Code->assemble_program($contents)
            or die "Unable to assemble program\n";

        $code = $prog->serialize;
    }

    unless ($code) {
        print STDERR "No output produced.\n";
        exit;
    }

    if ($outfile) {
        open($out, ">", $outfile) or die "Could not open $outfile for writing: $!\n";
        print $out $code;
        close $out;
    } else {
        print $code;
    }
}

sub usage {
    print "Usage: $0 [-d] infile [-o outfile]\n";
    exit;
}
