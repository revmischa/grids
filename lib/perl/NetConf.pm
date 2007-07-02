use strict;

package NetConf;
use Storable;
use Carp qw (croak);
use Data::Dumper;

sub new {
    my ($class, %opts) = @_;

    my $conffile = $opts{conf_file} or croak "No conf file";

    my $self = {
        conf_file => $conffile,
        conf => {},
    };

    bless $self, $class;

    return $self;
}

sub conf_file { $_[0]->{conf_file} }

sub save {
    my $self = shift;
    my $vars = $self->conf;
    Storable::nstore($vars, $self->conf_file);
}

sub load {
    my $self = shift;
    my $conffile = $self->conf_file;

    return undef unless -e $conffile;

    my $varsref = Storable::retrieve($conffile);
    $self->set_conf_vars(%$varsref);

    return 1;
}

sub set_conf {
    my ($self, $var, $val) = @_;

    return 0 unless exists $self->{conf}->{$var};
    $self->{conf}->{$var} = $val;
    return 1;
}

sub get_conf { $_[0]->{conf}->{$_[1]} }

sub conf_vars { keys %{$_[0]->{conf}} }

sub conf { $_[0]->{conf} }

sub set_conf_vars {
    my ($self, %vars) = @_;

    $self->set_conf($_ => $vars{$_}) foreach keys %vars;
}

sub add_conf { $_[0]->{conf}->{$_[1]} ||= $_[2] }

1;
