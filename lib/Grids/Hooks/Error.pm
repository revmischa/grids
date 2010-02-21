package Grids::Hooks::Error;

use Moose;
use Grids::Hooks;

# register global error handler to print errors to stderr
Grids::Hooks->register_hooks(
                             qr/^Error/ => \&error,
                             );

sub error {
    my ($client, $info) = @_;

    my $evt = delete $info->{event};

    warn "Caught error: " . join "\n", map { "$_ = $info->{args}{$_}\n" } keys %{$info->{args}};

    return 0;
}

no Moose;
__PACKAGE__->meta->make_immutable;

