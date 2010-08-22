package Grids::Node::Hooks::Storage;

use Moose;
use namespace::autoclean;

# This hook provides a client interface to the node's client storage

my $prefix = 'Storage';

Grids::Node->register_hooks(
    "$prefix.Empty" => \&empty,
    "$prefix.List" => \&list,
    "$prefix.Put" => \&put,
    "$prefix.Get" => \&get,
    "$prefix.Delete" => \&delete,
);

sub empty {
    my ($node, $evt) = @_;
    $node->client_storage->empty($evt->peer);
    return;
}

sub list {
    my ($node, $evt) = @_;
    my $keys = $node->client_storage->list($evt->peer);
    return $node->hook_ok(keys => $keys);
}

sub put {
    my ($node, $evt) = @_;
    $node->client_storage->put($evt->peer, $evt->key, $evt->value);
    return;
}

sub get {
    my ($node, $evt) = @_;

    my $val = $node->client_storage->get($evt->peer, $evt->key);
    return $node->hook_ok(key => $evt->key, value => $val);
}

sub delete {
    my ($node, $evt) = @_;
    $node->client_storage->delete($evt->peer, $evt->key);
    return;
}


no Moose;
__PACKAGE__->meta->make_immutable;

