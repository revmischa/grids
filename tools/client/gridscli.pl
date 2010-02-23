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
my $debug;

my %prog_opts = (
                 'h|help'  => \$help,
                 'i|id'    => \$id_name,
                 'c|conf'  => \$conffile,
                 'dump'    => \$dump,
                 'd|debug' => \$debug,
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
                              newid    => \&create_id,
                              help     => \&help,
                              set      => \&Grids::Console::set,
                              save     => \&Grids::Console::save,
                              list     => \&Grids::Console::list,
                              services => \&list_services,
                              connect  => \&connect,
                              cl       => \&connect_localhost,
                              echo     => \&echo,
                          },
                          );

# load identity
my $identity = $con->interactively_load_identity($id_name);
unless ($identity) {
    $con->print_error("No identity specified.");

}

my $client = Grids::Client->new(
    debug     => $debug,
    conf      => $conf,
    id        => $identity,
    transport => 'TCP',
);

$client->register_hook('Services.List', sub {
    $con->print("Got services list");
});

$client->register_hook('Connected', sub {
    $con->print("Protocol established");
});

run();

sub connect_localhost {
    my ($con) = @_;
    ::connect($con, 'localhost');
}

sub connect {
    my ($con, $addr) = @_;

    my $tcp_addr = new Grids::Address::TCP(address => $addr);

    if ($client->connect($tcp_addr)) {
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

sub list_services {
    $client->dispatch_event('Services.List');
}

sub echo {
    my ($con, $msg) = @_;
    $client->dispatch_event('Debug.Echo', { message => $msg });
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
