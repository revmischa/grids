#!/usr/bin/perl
use strict;

# Command-line interface to the Grids client

use FindBin;
use lib "$FindBin::Bin/../../lib";

use Grids::Client;
use Grids::Identity;
use Grids::Console;
use Grids::Conf;
use Grids::Address::IPv4;

use AnyEvent;
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

# main loop condition
my $main = AnyEvent->condvar;

my $con = Grids::Console->new(
    cv => $main,
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
    exit 1;
}

my $client = Grids::Client->new(
    autosave_configuration => 1,
    autoload_configuration => 1,
    log_level              => $debug ? 5 : 3,
    conf                   => $conf,
    id                     => $identity,
    use_encryption         => 1,
    transport_driver       => 'TCP::AnyEvent',
);

$client->register_hook('Services.List', sub {
    $con->print("Got services list");
});

$client->register_hook('Connected', sub {
    $con->print("Connected");
});

$client->register_hook(qr//, sub {
    my (undef, $evt) = @_;

    return unless $debug;
    $con->print("Got " . $evt->name . " event: " .
        Dumper($evt));
});

run();


############


sub connect_localhost {
    my ($con) = @_;
    ::connect($con, 'localhost');
}

sub connect {
    my ($con, $addr) = @_;

    my $ip_addr = new Grids::Address::IPv4(address => $addr);

    $client->connect($ip_addr);
}

sub run {
    $con->listen_for_input;
    $main->recv;
}

sub create_id {
    $con->interactively_generate_identity;
}

sub list_services {
    $client->send_event('Services.List');
    return;
}

sub echo {
    my ($con, $msg) = @_;

    unless ($client->send_event('Echo', { echo_message => $msg })) {
        $con->print("ERROR: Failed to send event");
    }

    return;
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
