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

# return value (in $v0)
sub retval {
    my ($self, $val) = @_;
    $self->vm->set_reg('v0', $val);
}

# return error with optional message
sub ret_err {
    my ($self, $errno, $msg) = @_;

    $self->retval($errno);
    $self->vm->wrn($msg) if $msg;
}

1;
