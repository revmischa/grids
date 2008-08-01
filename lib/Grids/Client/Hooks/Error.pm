package Grids::Client;
use strict;
use warnings;

__PACKAGE__->register_hooks(
                            qr/^Error/ => \&error,
                            );

sub error {
    my ($client, %info) = @_;

    my $evt = delete $info{event};

    die "Caught error: " . join "\n", map { "$_ = $info{args}{$_}\n" } keys %{$info{args}};

    return 0;
}

1;
