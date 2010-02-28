# object representing configuration

package Grids::Conf;

use Moose;

use Storable;
use Carp qw (croak);
use Data::Dumper;

use constant {
    DEFAULT_FILE_NAME => '.gridsnode',
};

has conf_file => (
    is => 'rw',
    isa => 'Maybe[Str]',
    default => sub {
        DEFAULT_FILE_NAME
    },
);

has conf => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

has loaded => (
    is => 'rw',
    isa => 'Bool',
);

sub file_name {
    my ($self) = @_;
    return $self->conf_file || DEFAULT_FILE_NAME;
}

sub save {
    my $self = shift;

    my $vars = $self->conf;

    $self->dump;

    my $file_name = $self->file_name;
    Storable::nstore($vars, $file_name);
}

sub load {
    my $self = shift;

    my $file_name = $self->file_name;

    return undef unless $file_name && -e $file_name;

    my $varsref = Storable::retrieve($file_name);
    $self->loaded($varsref ? 1 : 0);
    $self->set_conf_vars(%$varsref);

    $self->dump;

    return 1;
}

sub dump {
    my $self = shift;
    warn Dumper($self->conf);
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
# it already exists. useful for setting default values.
*default = \&add_conf;
sub add_conf { $_[0]->conf->{$_[1]} ||= $_[2] }

no Moose;
__PACKAGE__->meta->make_immutable;
