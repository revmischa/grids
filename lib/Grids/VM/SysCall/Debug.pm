package Grids::VM::SysCall::Debug;

use Moose;
    with 'Grids::VM::SysCall';

sub BUILD {
    my ($self) = @_;

    print "SysCall::Debug loaded\n";

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

    printf("[debug.lognum] %032b %s %u 0x%08X\n",
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

    printf "[debug.logstrz] %s\n", $str;
}

# prints string at addr in $a1, length $a2
sub logstr {
    my ($self) = @_;

    my $straddr = $self->vm->reg_u('a1');
    my $strlen  = $self->vm->reg_u('a2');

    my $str = pack("A$strlen", $self->vm->get_mem($straddr++, $strlen));

    printf "[debug.logstr] %s\n", $str;
}

no Moose;
__PACKAGE__->meta->make_immutable;

