# object representing configuration

package Grids::Conf;

use Moose;

use Storable;
use Carp qw (croak);
use Data::Dumper;

has conf_file => (
    is => 'rw',
);

has conf => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

sub save {
    my $self = shift;
    my $vars = $self->conf;
    Storable::nstore($vars, $self->conf_file);
}

sub load {
    my $self = shift;
    my $conffile = $self->conf_file;

    return undef unless $conffile && -e $conffile;

    my $varsref = Storable::retrieve($conffile);

    $self->set_conf_vars(%$varsref);

    return 1;
}

*set = \&set_conf;

# sets new value, creating the conf var if necessary
sub set_conf { $_[0]->conf->{$_[1]} = $_[2] }

# only sets a variable if it exists, doesn't create new vars
sub set_conf_if_exists {
    my ($self, $var, $val) = @_;

    return 0 unless exists $self->conf->{$var};
    $self->conf->{$var} = $val;
    return 1;
}

*get = \&get_conf;
sub get_conf { $_[0]->conf->{$_[1]} }

sub conf_vars { keys %{$_[0]->conf} }

sub set_conf_vars {
    my ($self, %vars) = @_;

    $self->set_conf($_ => $vars{$_}) foreach keys %vars;
}

# creates a new conf with a value if it doesn't exist, does nothing if
# it already exists
sub add_conf { $_[0]->conf->{$_[1]} ||= $_[2] }

no Moose;
__PACKAGE__->meta->make_immutable;
