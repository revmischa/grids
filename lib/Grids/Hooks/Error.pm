package Grids::Hooks::Error;

use Moose;
use Grids::Hooks;

# register global error handler to print errors to stderr
Grids::Hooks->register_hooks(
    qr/^Error/ => \&error,
);

sub error {
    my ($base, $evt) = @_;

    my $args = $evt->args;

    $base->warn("Caught error: " . join("\n", map { "$_ = $args->{$_}\n" } keys %$args));

    return 0;
}

no Moose;
__PACKAGE__->meta->make_immutable;

