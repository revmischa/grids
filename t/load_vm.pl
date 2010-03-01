# use locally built versions of memory/register for testing
use lib 'lib';
use lib 'lib/Grids/VM/Memory/lib';
use lib 'lib/Grids/VM/Register/lib';
use lib 'lib/Grids/VM/Memory/blib/arch/auto/Grids/VM/Memory';
use lib 'lib/Grids/VM/Register/blib/arch/auto/Grids/VM/Register';

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

1;
