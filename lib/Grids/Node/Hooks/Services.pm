package Grids::Node;
use strict;
use warnings;

__PACKAGE__->register_hooks(
                            'Services.List' => \&hook_services_list,
                            );

sub hook_services_list {
    my ($node, %info) = @_;

    return 0 unless $node->check_authentication(\%info);

    my @services = $node->services;

    return { services => \@services };
}

1;
