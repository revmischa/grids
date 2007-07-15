#!/usr/bin/perl
use strict;

use lib '../../lib/perl';
use NetVM;
use NetConsole;
use NetConf;
use NetNode;

use Getopt::Long;
use Sys::Hostname;
use sigtrap qw(die normal-signals);

my $conffile = 'netnode.conf';
my $nodename = hostname;
my $help;

my %prog_opts = (
                 'h|help' => \$help,
                 'n|name' => \$nodename,
                 'c|conf' => \$conffile,
                 );

GetOptions(%prog_opts);

# keep track of child listening processes
my %children;
$SIG{CHLD} = 'IGNORE';

$SIG{USR1} = sub {
    exit 0;
};

my $conf = NetConf->new(conf_file => $conffile);

my $node;
my $con = NetConsole->new(
                          conf => $conf,
                          title => "NetNode",
                          prompt => "NetNode [$nodename]> ",
                          handlers => {
                              help  => \&help,
                              set   => \&NetConsole::set,
                              save  => \&NetConsole::save,
                              list  => \&NetConsole::list,
                          },
                          );

run();

sub run {
    $node = NetNode->new(conf => $conf);

    $con->print("Loaded settings from $conffile") if $conf->load;

    my $trans = $node->add_transport("TCP");
    if(my $chpid = fork()) {
        $children{$chpid} = 1;
    } else {
        $trans->accept_loop;
        exit 0;
    }

    $con->run;
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

