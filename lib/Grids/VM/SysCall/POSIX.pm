package Grids::VM::SysCall::POSIX;

use Moose;
    with 'Grids::VM::SysCall';

use POSIX;

has errno => (
    is => 'rw',
    isa => 'Int',
    default => sub { 0 },
);

sub BUILD {
    my ($self) = @_;

    $self->register(
        4 => \&write,
    );
}

sub _ret_err {
    my ($self, $errno, $msg) = @_;
    $self->errno($errno);
    $self->vm->set_reg('v0', -1);
    $self->vm->wrn($msg) if $msg;
}

# prints string at addr in $a1, length $a2 to open file descriptor $a0
sub write {
    my ($self) = @_;

    my $vm = $self->vm;

    my $fdno    = $vm->reg_u('a0');
    my $straddr = $vm->reg_u('a1');
    my $strlen  = $vm->reg_u('a2');

    my $str = pack("a$strlen", $vm->get_mem($straddr, $strlen));

    if ($fdno == 1) {
        # STDOUT
        printf "[posix.write] %s\n", $str;
        $vm->set_reg('v0', $strlen);
    } else {
        return $self->_ret_err(ENXIO, "tried to write to unknown file descriptor $fdno");
    }
}

no Moose;
__PACKAGE__->meta->make_immutable;

