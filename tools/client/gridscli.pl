#!/usr/bin/perl
use strict;

# Command-line interface to the Grids client

use FindBin;
use lib "$FindBin::Bin/../../lib";

use Grids::Client;
use Grids::Identity;
use Grids::Console;
use Grids::Conf;

use Carp qw (croak);
use Getopt::Long;
use Data::Dumper;

############

my $conffile = 'gridsclient.conf';
my $id_name;
my $help;
my $dump;

my %prog_opts = (
                 'h|help' => \$help,
                 'i|id'   => \$id_name,
                 'c|conf' => \$conffile,
                 'd|dump' => \$dump,
                 );

GetOptions(%prog_opts);

my $conf = Grids::Conf->new(conf_file => $conffile);
print "Loaded settings from $conffile\n" if $conf->load;

die Dumper($conf) if $dump;

my $con = Grids::Console->new(
                          conf => $conf,
                          title => "Grids",
                          prompt => "Grids> ",
                          handlers => {
                              newid   => \&create_id,
                              help    => \&help,
                              set     => \&Grids::Console::set,
                              save    => \&Grids::Console::save,
                              list    => \&Grids::Console::list,
                              connect => \&connect,
                          },
                          );

# load identity
my $identity = $con->interactively_load_identity($id_name);
unless ($identity) {
    $con->print_error("No identity specified.");

}

my $client = Grids::Client->new(
                            conf      => $conf,
                            id        => $identity,
                            transport => 'TCP',
                            );

run();

sub connect {
    my ($con, $addr) = @_;

    if ($client->connect($addr)) {
        return "Connected to $addr";
    } else {
        return "Failed to connect to $addr";
    }
}

sub run {
    $con->run;
}

sub create_id {
    $con->interactively_generate_identity;
}

sub help {
    my @args = @_;

    return q {
connect - connect to a node
set     - set/view client variables
save    - save settings
list    - show all variables
newid   - create a new identity
quit    - quit
};
    }
