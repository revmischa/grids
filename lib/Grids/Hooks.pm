# class to store global hooks on for Nodes and Clients

package Grids::Hooks;

use strict;
use warnings;
use base qw/Grids::Hookable/;

our %HOOKS;

sub register_hook {
    my ($self, $hookname, $cb, $cb_obj) = @_;

    my $hook_save = $self->hook_save($hookname, $cb, $cb_obj);

    $HOOKS{$hookname} ||= [];
    push @{$HOOKS{$hookname}}, $hook_save;
}

1;
