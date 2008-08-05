use strict;

package Grids::Console;

use Grids::Identity;
use Term::ReadLine;
use Storable;
use Carp qw (croak);

sub new {
    my ($class, %opts) = @_;

    my $conf = $opts{conf};
    my $handlers = $opts{handlers} || {};
    my $prompt = $opts{prompt} || '>';
    my $title = $opts{title} || 'Grids';
    my $msg = $opts{message};

    my $term = new Term::ReadLine $title;
    my $OUT = $term->OUT || \*STDOUT;

    my $self = {
        prompt => $prompt,
        title  => $title,
        conf   => $conf,
        term   => $term,
        OUT    => $OUT,
        msg    => $msg,
    };

    bless $self, $class;

    $self->set_handler($_, $handlers->{$_}) foreach keys %$handlers;

    return $self;
}

sub conf { $_[0]->{conf} }
sub term { $_[0]->{term} }

sub run {
    my $self = shift;

    my $prompt = $self->{prompt};

    $self->print("\n$self->{msg}") if $self->{msg};

    while (defined (my $line = $self->term->readline($prompt))) {
        if ($line =~ /^\s*(q|quit|exit)\b/ig) {
            last;
        }

        my $res = eval {
            $self->do_command($line);
        };

        if ($@) {
            $self->print("Error: $@");
        } else {
            $self->print($res) if $res;
            $self->term->addhistory($line) if $line =~ /\S/;
        }
    }
}

sub yesorno {
    my ($self, $q, $default) = @_;

    $default ||= 'Y';
    $default = lc $default;

    my $defstr;
    if ($default eq 'y') {
        $defstr = 'Y/n';
    } elsif ($default eq 'n') {
        $defstr = 'N/y';
    } else {
        croak "default to yesorno must be '', 'y' or 'n'";
    }

    my $resp = $self->ask("$q [$defstr] ");
    $resp = $default if $resp =~ /^\s*$/;

    my $yes = $resp =~ /^\s*y/i;
    return $yes;
}

sub ask {
    my ($self, $txt) = @_;
    my $line = $self->term->readline($txt);
    chomp $line;
    return $line;
}

sub print {
    my ($self, $txt) = @_;
    my $OUT = $self->{OUT};
    print $OUT $txt . "\n";
}

sub set_handler {
    my ($self, $cmd, $handler) = @_;

    $self->{handlers}->{$cmd} = $handler;
}

sub do_command {
    my ($self, $input) = @_;

    my ($cmd, $args) = $input =~ /^\s*(\w+)\s*(.+)?\s*$/sm;
    my @args = split(/\s+/, $args);

    return "" unless $cmd;

    $cmd = lc $cmd;

    my $func = $self->{handlers}->{$cmd} or die "No such command: $cmd\n";

    return $func->($self, @args);
}

sub set {
    my ($self, $var, $val) = @_;

    if (! $var) {
        return "Usage: set [\$variable] [\$value]\nDisplays current value of \$variable if \$value is not defined";
    } elsif (! $val) {
        $val = $self->conf->get_conf($var);
        return "$var: $val";
    } else {
        my $old_val = $self->conf->get_conf($var);
        if ($self->conf->set_conf($var, $val)) {
            return "$var: old value: $old_val new value: $val";
        } else {
            return "No such variable $var";
        }
    }
}

# show all variables
sub list {
    my $self = shift;

    my $ret = "Variables:\n" . '-' x 10 . "\n";

    my @vars = $self->conf->conf_vars;
    foreach my $var (@vars) {
        $ret .= "$var: " . $self->conf->get_conf($var) . "\n";
    }

    $ret .= '-' x 10;

    return $ret;
}

sub print_error {
    my ($self, $err) = @_;

    $self->print("Error: $err");
}

sub print_message {
    my ($self, $msg) = @_;
    
    $self->print(" -- $msg --");
}

sub interactively_load_identity {
    my ($self, $id_name) = @_;

    my $conf = $self->conf;

    my $ids = $conf->get('id') || {};
    my $id;

    if ($id_name) {
        unless (%$ids) {
            $self->print_error("There are no saved identities");
            $id = $self->interactively_generate_identity;
        }

        unless ($id = $ids->{$id_name}) {
            $self->print_error("There is no saved identity named \"$id_name\"");
            $id = $self->interactively_generate_identity;
        }
    }

    if (%$ids && ! $id) {
        # ids exist, but none specified
        if ((scalar keys %$ids) == 1) {
            # if only one id exists, use that
            my $name;
            ($name, $id) = %$ids;
            $self->print("Loaded identity '$name'");
        } else {
            $self->print_error("FIXME: let user choose identity");
        }
    }

    if ($id) {
        $id = Grids::Identity->deserialize($id) or die "Error loading identity\n";
    }

    if ($id && $id->encrypted) {
        my $name = $id->name;

        my $passphrase = $self->ask("Passphrase needed for identity '$name': ");

        if ($passphrase) {
            my $decrypted = $id->keys_match;
            #my $decrypted = $id->decrypt_privkey($passphrase);

            if ($decrypted) {
                $self->print("Identity decrypted");
            } else {
                $self->print("Incorrect passphrase. Identity not loaded");
                $id = undef;
            }
        }
    }

    # didn't find an id to load, ask if they want to generate one
    unless ($id) {
        # no id specified, ask to create one
        my $create = $self->yesorno("No identity specified and there are no saved identities. " .
                                    "Would you like to create one?");

        $id = $self->interactively_generate_identity if $create;
    }

    return $id;
}

sub interactively_generate_identity {
    my $con = shift;
    my $conf = $con->conf;

    $con->print_message('Generating new identity');
    my $name = $con->ask("What personal identifier would you like to give this identity? [default] ") || 'default';
    my $passphrase = $con->ask("Enter identity passphrase (leave blank for no passphrase): ");

    # TODO: make passphrase work
    my $id = Grids::Identity->create(passphrase => $passphrase, name => $name, verbose => 1);

    $name ||= 'default';
    
    my $ids = $conf->get('id') || {};

    if ($ids->{$name}) {
        my $overwrite = $con->ask("Identity '$name' already exists. Overwrite it?");
        return unless $overwrite;
    }

    my $id_ser = $id->serialize;
    $ids->{$name} = $id_ser;
    $conf->set('id', $ids);

    if ($con->yesorno("Identity generated and added to your configuration. Would you like to write your configuration to disk? ")) {
        $conf->save;
        print "Saved\n";
    }

    return $id;
}

sub save {
    my $self = shift;
    $self->conf->save;
    return "saved settings to " . $self->conf->conf_file;
}

1;
