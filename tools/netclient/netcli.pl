#!/usr/bin/perl
use strict;

# Command-line interface to the Net client

use lib '../../lib/perl';
use NetClient;
use NetIdentity;
use NetConsole;
use NetConf;

use Carp qw (croak);
use Getopt::Long;

############

my $conffile = 'netclient.conf';
my $id;
my $help;

my %prog_opts = (
                 'h|help' => \$help,
                 'i|id'   => \$id,
                 'c|conf' => \$conffile,
                 );

GetOptions(%prog_opts);

my $conf = NetConf->new(conf_file => $conffile);
$id ||= $conf->get('id');

my $con = NetConsole->new(
                          conf => $conf,
                          title => "Net",
                          prompt => "Net> ",
                          handlers => {
                              help  => \&help,
                              set   => \&NetConsole::set,
                              save  => \&NetConsole::save,
                              list  => \&NetConsole::list,
                          },
                          );

$id = create_id() unless $id;

my $client = NetClient->new(
                            conf      => $conf,
                            id        => $id,
                            transport => 'TCP',
                            );

run();

sub run {
    $con->print("Loaded settings from $conffile") if $conf->load;
    $con->run;
}

# user didn't specify an id and there were none in the conf file
sub create_id {
    my $create = $con->ask("No identity specified and there are no saved identities. " .
                           "Would you like to create one [Y/n]? ");

    return unless $create eq '' || $create =~ /^\s*y/i;

    my $name = $con->ask("What identifier would you like to give this identity? ") || '';
    my $passphrase = $con->ask("Enter id passphrase (leave blank for no passphrase): ") || '';
    my $id = NetIdentity->create(passphrase => $passphrase, name => $name, verbose => 1);

    my $id_ser = $id->serialize;
}

sub help {
    my @args = @_;

    return q {
connect - connect to a node
set     - set/view client variables
save    - save settings
list    - show all variables
quit    - quit
};
    }
