package Grids::Client;
use strict;
use warnings;

__PACKAGE__->register_event_hooks(
                                  qr/Protocol\.Error\.(.*)/ => \&hook_protocol_error,
                                  );

sub hook_protocol_error {
    my ($client, %info) = @_;

    my $evt = delete $info{event};

    die "Protocol error: " . join "\n", map { "$_ = $info{args}{$_}\n" } keys %{$info{args}};

    return 0;
}

1;
