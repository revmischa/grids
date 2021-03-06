#!/usr/bin/perl

use strict;
use warnings;

use FindBin;
use lib "$FindBin::Bin/../../lib";
use lib "$FindBin::Bin/../../lib/Grids/VM/Memory/lib";
use lib "$FindBin::Bin/../../lib/Grids/VM/Memory/blib/arch/auto/Grids/VM/Memory";
use lib "$FindBin::Bin/../../lib/Grids/VM/Register/lib";
use lib "$FindBin::Bin/../../lib/Grids/VM/Register/blib/arch/auto/Grids/VM/Register";

use Grids::VM;
use Grids::Console;
use Grids::Conf;
use Grids::Node;

use Getopt::Long;
use Sys::Hostname;
use AnyEvent;

my $conffile;
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

# main loop condition
my $main = AnyEvent->condvar;

my $con = Grids::Console->new(
    cv => $main,
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

sub client_connected {
    my ($node, $connection, $peer_name) = @_;
    $con->print("Client $peer_name connected, encrypted transport enabled");
}

sub client_disconnected {
    my ($node, $connection, $peer_name) = @_;
    $con->print("Client $peer_name disconnected, encrypted transport disabled");
}

sub run {
    $con->print("Loaded settings from " . $conf->file_name) if $conf->loaded;

    # get identity
    my $identity = $con->interactively_load_identity;
    unless ($identity) {
        $con->print_error("No identity specified");
        exit 0;
    }
    
    # create node
    my $node = Grids::Node->new(
        conf => $conf,
        log_level => $debug ? 5 : 3,
        id => $identity,
        transport_driver => 'TCP::AnyEvent',
        autosave_configuration => 1,
        autoload_configuration => 1,
    );

    # register hooks here
    $node->register_hook('Connected', \&client_connected);
    $node->register_hook('Disconnected', \&client_disconnected);

    # listen for connections
    $node->listen;

    # listen for user input
    $con->listen_for_input;

    local $SIG{INT} = sub {
        $main->send;
    };

    # main loop
    $main->recv;
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
usage: $0 [-cnihd]
 -c[onf]:  specify a configuration file. default is "gridsnode.conf"
 -n[ame]:  specify node name
 -d[ebug]: enable debug output
 -i[d]:    specify an identity to use
 -h[help]: print this help
};
}
