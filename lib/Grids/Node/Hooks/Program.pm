package Grids::Node::Hooks::Program;

use Moose;
use namespace::autoclean;

### This is an interface for compiling and running programs on a Node

my $prefix = 'Program';
Grids::Node->register_hooks(
    "$prefix.CompileAndExecute" => \&compile_and_execute,
);

# load source code from client storage, compile it and run it
sub compile_and_execute {
    my ($node, $evt) = @_;

    # load source
    my $storage_key = $evt->source_storage_key;
    my $source = $node->client_storage->get($evt->peer, $storage_key);
    unless ($source) {
        return $node->hook_error("Error.Storage.KeyNotFound");
    }

    # compile
    my $program = eval {
        Grids::Code->assemble_program($source);
    };
    unless ($program) {
        return $node->hook_error("Error.Program.CompileError",
            error => "Compile error: " . ($@ || '(unknown error)'),
        );
    }

    # run program
    $node->run_program($program);

    return;
}

no Moose;
__PACKAGE__->meta->make_immutable;

