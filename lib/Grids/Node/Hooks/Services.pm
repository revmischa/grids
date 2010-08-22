package Grids::Node::Hooks::Services;

use Moose;
use Grids::Node;

Grids::Node->register_hooks(
    'Services.List' => \&hook_services_list,
);

sub hook_services_list {
    my ($node, $event) = @_;

    return 0 unless $node->check_authentication($event);

    my @services = $node->services;

    return { services => \@services };
}

no Moose;
__PACKAGE__->meta->make_immutable;
