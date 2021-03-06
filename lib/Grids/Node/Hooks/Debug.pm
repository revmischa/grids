package Grids::Hooks::Debug;

use Moose;
use Grids::Node;

Grids::Node->register_hooks(
    "Debug.Warn" => \&warn,
    "Debug.Echo" => \&echo,
);

sub warn {
    my ($node, $evt) = @_;

    $node->warn("\nGot Debug.Warn: " .
                $evt->args->{message} . "\n");
    return undef;
}

sub echo {
    my ($node, $evt) = @_;

    $node->warn("\nGot Debug.Echo: " .
                $evt->args->{message} . "\n");
    return $node->event_hook_success(%{$evt->args});
}

no Moose;
__PACKAGE__->meta->make_immutable;
