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
use Data::Dumper;

############

my $conffile = 'netclient.conf';
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

my $conf = NetConf->new(conf_file => $conffile);
print "Loaded settings from $conffile\n" if $conf->load;

die Dumper($conf) if $dump;

my $ids = $conf->get('id') || {};
my $id;

if ($id_name) {
    die "There are no saved identities\n" unless %$ids;
    $id = $ids->{$id_name} or die "There is no saved identity named \"$id_name\"\n";
}

if (%$ids && ! $id) {
    # ids exist, but none specified
    if ((scalar keys %$ids) == 1) {
        # if only one id exists, use that
        my $name;
        ($name, $id) = %$ids;
        print "Loaded identity '$name'\n";
    } else {
        # have user choose id
        # TODO
    }
}

my $con = NetConsole->new(
                          conf => $conf,
                          title => "Net",
                          prompt => "Net> ",
                          handlers => {
                              newid => \&create_id,
                              help  => \&help,
                              set   => \&NetConsole::set,
                              save  => \&NetConsole::save,
                              list  => \&NetConsole::list,
                          },
                          );

if ($id) {
    # load serialized id
    $id = NetIdentity->deserialize($id) or die "Error loading identity\n";
    die "Invalid identity\n" unless $id->privkey->check;

    if ($encrypted) {
        my $decrypted = decrypt($id);
        if ($decrypted) {
            print "Identity decrypted\n";
        } else {
            print "Incorrect passphrase. Identity not loaded\n";
            $id = undef;
        }
    }
}

unless ($id) {
    # no id specified, ask to create one
    my $create = $con->yesorno("No identity specified and there are no saved identities. " .
                               "Would you like to create one?");

    $id = create_id() if $create;

    die "You must have an identity to use this program.\n" unless $id;
}

my $client = NetClient->new(
                            conf      => $conf,
                            id        => $id,
                            transport => 'TCP',
                            );

run();

sub run {
    $con->run;
}

# user didn't specify an id and there were none in the conf file
sub create_id {
    my $name = $con->ask("What personal identifier would you like to give this identity? ") || 'default';
    my $passphrase = $con->ask("Enter id passphrase (leave blank for no passphrase): ") || '';
    my $id = NetIdentity->create(passphrase => $passphrase, name => $name, verbose => 1);

    my $id_ser = $id->serialize;
    $name ||= 'default';
    
    my $ids = $conf->get('id') || {};

    if ($ids->{$name}) {
        my $overwrite = $con->ask("Identity '$name' already exists. Overwrite it?");
        return unless $overwrite;
    }

    $ids->{$name} = $id_ser;
    $conf->set('id', $ids);

    if ($con->yesorno("Identity generated. Do you want to save it? ")) {
        $conf->save;
        print "Saved\n";
    }

    $id->decrypt_privkey($passphrase) if $passphrase; # is this necessary?

    return $id;
}

sub decrypt {
    my $id = shift;
    
    my $name = $id->name;

    my $passphrase = $con->ask("Passphrase needed for identity '$name': ")
        or return 0;

    return $id->decrypt_privkey($passphrase);
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
