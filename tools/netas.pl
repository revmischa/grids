#!/usr/bin/perl
use strict;
use lib 'lib/perl';
use NetCode;
use Getopt::Long;

my $outfile;

GetOptions("o|output=s" => \$outfile);

my $infile = shift;

usage() unless $infile && $outfile;

die "$infile does not exist\n" unless -e $infile;

# read file in and assemble
{
    my ($in, $out, $contents);

    open($in, $infile) or die "Could not open $infile for reading: $!\n";
    local $/; # slurrrpp
    $contents = <$in>;
    close $in;

    my $bytecode = NetCode->assemble($contents);

    unless ($bytecode) {
        print STDERR "No assembly output produced. Output not written.\n";
        exit;
    }

    open($out, ">", $outfile) or die "Could not open $outfile for writing: $!\n";
    print $out $bytecode;
    close $out;

    print "Success.\n";
}

sub usage {
    print "Usage: $0 infile -o outfile\n";
    exit;
}
