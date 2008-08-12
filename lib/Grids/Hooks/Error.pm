package Grids::Hooks::Error;
use strict;
use warnings;

Grids::Hooks->register_hooks(
                             qr/^Error/ => \&error,
                             );

sub error {
    my ($client, $info) = @_;

    my $evt = delete $info->{event};

    warn "Caught error: " . join "\n", map { "$_ = $info->{args}{$_}\n" } keys %{$info->{args}};

    return 0;
}

1;
