package Grids::VM::SysCall;

use Moose::Role;

# virtual machine
has vm => (
    is => 'ro',
    isa => 'Grids::VM',
    required => 1,
    handles => {
        base => 'base',
    },
);

# register system calls with the VM
sub register {
    my ($self, %handlers) = @_;

    foreach my $syscall (keys %handlers) {
        # bind to handler as an instance method
        my $handler = $handlers{$syscall};
        my $cb = sub {
            $self->$handler(@_);
        };

        $self->vm->register_syscall($syscall => $cb);
    }
}

1;
