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

sub _err {
    my ($self, $errno, $msg) = @_;

    $self->errno($errno);
    $self->ret_err(-1, $msg);
};

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
        return $self->retval($strlen);
    } else {
        return $self->_err(ENXIO, "tried to write to unknown file descriptor $fdno");
    }
}

no Moose;
__PACKAGE__->meta->make_immutable;

