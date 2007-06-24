#!/usr/bin/perl
use strict;
use lib '../../lib/perl';
use NetVM;
use Term::ReadLine;
use NetNode;
use Getopt::Long;
use Sys::Hostname;
use File::UserConfig;
use sigtrap qw(die normal-signals);

my ($help, $nodename);
my %prog_opts = (
                 'h|help' => \$help,
                 'n|name' => \$nodename,
                 );
GetOptions(%prog_opts);
$nodename ||= hostname;

my %children;
$SIG{CHLD} = 'IGNORE';

$SIG{USR1} = sub {
    exit 0;
};

my $node = NetNode->new();
my $trans = $node->add_transport("TCP");
if(my $chpid = fork()) {
    $children{$chpid} = 1;
} else {
    $trans->accept_loop;
    exit 0;
}
run();

sub run {
    my $term = new Term::ReadLine 'NetVM';
    my $OUT = $term->OUT || \*STDOUT;
    my $infile = shift();

    load_settings();

    my $prompt = "NetNode [$nodename]> ";

    while (defined (my $line = $term->readline($prompt))) {
        if ($line =~ /^\s*(q|quit|exit)\b/ig) {
            last;
        }

        my $res = eval {
            do_command($line);
        };

        if ($@) {
            print $OUT "Error: $@\n";
        } else {
            print $OUT $res . "\n" if $res;
            $term->addhistory($line) if $line =~ /\S/;
        }
    }
}

sub do_command {
    my $input = shift();

    my ($cmd, $args) = $input =~ /^\s*(\w+)\s*(.+)?\s*$/sm;
    my @args = split(/\s*/, $args);

    return "" unless $cmd;

    $cmd = lc $cmd;

    my %handlers = (
                    help => \&help,
                    set  => \&set,
                    save => \&save,
                    list => \&list,
                    );

    my $func = $handlers{$cmd} or die "No such command: $cmd\n";

    return $func->(@args);
}

sub set {
    my ($var, $val) = @_;

    if (! $var) {
        return "Usage: set [\$variable] [\$value]\nDisplays current value of \$variable if \$value is not defined";
    } elsif (! $val) {
        $val = $node->get_conf($var);
        return "$var: $val";
    } else {
        my $old_val = $node->get_conf($var);
        if ($node->set_conf($var, $val)) {
            return "$var: old value: $old_val new value: $val";
        } else {
            return "No such variable $var";
        }
    }
}

# save settings
sub save {

}

sub load_settings {

}

# show all variables
sub list {
    my $ret = "Variables:\n" . '-' x 10 . "\n";

    my @vars = $node->conf_vars;
    foreach my $var (@vars) {
        $ret .= "$var: " . $node->get_conf($var) . "\n";
    }

    $ret .= '-' x 10;

    return $ret;
}

sub help {
    my @args = @_;

    return q {
set  - set/view server variables
save - save settings
list - show all variables
quit - quit
};
}

END {
    foreach my $chpid (keys %children) {
        kill 'USR1', $chpid;
    }
}

