package Grids::VM::SysCall::Debug;

use Moose;
    with 'Grids::VM::SysCall';

sub BUILD {
    my ($self) = @_;

    $self->vm->dbg("SysCall::Debug loaded");

    $self->register(
        512 => \&logstr,
        513 => \&logstrz,
        514 => \&lognum,
    );
}

# prints value in $a0
sub lognum {
    my ($self) = @_;

    my $vm = $self->vm;
    $vm->dbg("[debug.lognum] %032b %s %u 0x%08X",
             $vm->reg('a0'), $vm->reg('a0'), $vm->reg('a0'), $vm->reg('a0'));
}

# prints null-terminated string at addr in $a1
sub logstrz {
    my ($self) = @_;

    my $straddr = $self->vm->reg_u('a1');

    my $str = '';

    while (my $c = $self->vm->get_mem($straddr++, 1)) {
        $str .= chr($c);
    }

    $self->vm->dbg(sprintf "[debug.logstrz] %s", $str);
}

# prints string at addr in $a1, length $a2
sub logstr {
    my ($self) = @_;

    my $straddr = $self->vm->reg_u('a1');
    my $strlen  = $self->vm->reg_u('a2');

    my $str = pack("A$strlen", $self->vm->get_mem($straddr++, $strlen));
    $self->vm->dbg(sprintf "[debug.logstr] %s", $str);
}

no Moose;
__PACKAGE__->meta->make_immutable;

