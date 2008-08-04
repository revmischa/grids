#!/usr/bin/perl
use strict;

use lib '../../lib';
use Grids::VM;
use Grids::Console;
use Grids::Conf;
use Grids::Node;

use Getopt::Long;
use Sys::Hostname;
use sigtrap qw(die normal-signals);
use threads;
use threads::shared;

my $conffile = 'gridsnode.conf';
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

my $conf = Grids::Conf->new(conf_file => $conffile);

my $node;
my $con = Grids::Console->new(
                          conf => $conf,
                          title => "GridsNode",
                          prompt => "GridsNode [$nodename]> ",
                          handlers => {
                              help  => \&help,
                              set   => \&Grids::Console::set,
                              save  => \&Grids::Console::save,
                              list  => \&Grids::Console::list,
                          },
                          );

run();

sub run {
    # get identity
    my $identity = $con->interactively_load_identity;
    unless ($identity) {
        $con->print_error("No identity specified");
        exit 0;
    }

    $node = Grids::Node->new(conf => $conf, debug => $debug, identity => $identity);

    $con->print("Loaded settings from $conffile") if $conf->load;

    my $finished : shared;

    my $trans = $node->add_transport("TCP");

    # run socket reading/event queue processing in seperate thread
    my $work_thread = async {
        $trans->listen;

        while (! $finished) {
            $trans->select;
            $node->flush_event_queue;
            threads->yield;
        }

        $trans->close_all;
        warn "work thread finished";
    };

    my $finish = sub {
        return if $finished;
        $finished = 1;
        $work_thread->join;
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
 -c[onf]:  specify a configuration file. default is "gridsnode.conf"
 -n[ame]:  specify node name
 -i[d]:    specify an identity to use
 -h[help]: print this help
};
}
