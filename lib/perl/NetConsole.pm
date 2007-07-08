use strict;

package NetConsole;
use Term::ReadLine;
use Storable;
use Carp qw (croak);

sub new {
    my ($class, %opts) = @_;

    my $conf = $opts{conf};
    my $handlers = $opts{handlers} || {};
    my $prompt = $opts{prompt} || '>';
    my $title = $opts{title} || 'Net';
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

sub save {
    my $self = shift;
    $self->conf->save;
    return "saved settings to " . $self->conf->conf_file;
}

1;
