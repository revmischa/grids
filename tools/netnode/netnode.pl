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
use threads;
use threads::shared;

my $conffile = 'netnode.conf';
my $nodename = hostname;
my ($help, $id, $debug);

my %prog_opts = (
                 'h|help'  => \$help,
                 'n|name'  => \$nodename,
                 'c|conf'  => \$conffile,
                 'i|id'    => \$id,
                 'd|debug' => \$debug,
                 );

GetOptions(%prog_opts);

usage() and exit if $help;

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
    $node = NetNode->new(conf => $conf, debug => $debug);

    $con->print("Loaded settings from $conffile") if $conf->load;

    my $finished : shared;

    my $trans = $node->add_transport("TCP");

    # run event queue processing in seperate thread
    my $evt_thread = async {
        while (! $finished) {
            $node->flush_event_queue;
            threads->yield;
        }
        warn "event thread finished";
    };

    my $select_thread = async {
        while (! $finished) {
            $trans->select;
            threads->yield;
        }

        $trans->close_all;
        warn "accept thread finished";
    };

    my $finish = sub {
        return if $finished;
        $finished = 1;
        $evt_thread->join;
        $select_thread->join;
        exit 0;
    };

    local $SIG{INT} = $finish;

    $con->run;
    $finish->();
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

sub usage {
    my @args = @_;
    print qq {
usage: $0 [-cnih]
 -c[onf]:  specify a configuration file. default is "netnode.conf"
 -n[ame]:  specify node name
 -i[d]:    specify an identity to use
 -h[help]: print this help
};
}
