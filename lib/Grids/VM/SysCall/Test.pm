# this is a set of system calls that can be used for running automated
# unit tests
package Grids::VM::SysCall::Test;

use Moose;
    with 'Grids::VM::SysCall';

require 'include/grids_posix.h';

sub BUILD {
    my ($self) = @_;

    $self->register(
        &Grids::POSIX::sys_fake_read => \&fake_read,
    );

    $self->vm->dbg("SysCall::Test loaded");
}

# emulate read() on STDIN
sub fake_read {
    my ($self) = @_;

    my $vm      = $self->vm;
    my $fdno    = $vm->reg_u('a0');
    my $bufaddr = $vm->reg_u('a1');
    my $bufsize = $vm->reg_u('a2');

    my $fakebuf = 'A' x $bufsize;
    my $readbuf = substr($fakebuf, 0, $bufsize);
    $vm->set_mem($bufaddr, $readbuf, $bufsize);
    return $self->retval($bufsize);
}

no Moose;
__PACKAGE__->meta->make_immutable;

