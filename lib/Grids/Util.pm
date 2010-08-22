package Grids::Util;

use Moose;
use namespace::autoclean;

# return path to root of Grids directory
sub base_dir {
    my ($class) = @_;
    my $module_path = $class->_module_path;
    my ($base_dir) = $module_path =~ m!^(.+)/lib/Grids/Util.pm!;
    return $base_dir;
}

sub _module_path {
    (caller())[1]; # path to current module
}

__PACKAGE__->meta->make_immutable;
