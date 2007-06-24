use strict;
package NetVM::SysCall::Node;

# prints value in $a1
sub log {
    my ($vm) = @_;

    printf "[Node.log] %08X\n", $vm->reg('a1');
}

# prints null-terminated string at addr in $a1
sub logstr {
    my ($vm) = @_;

    my $straddr = $vm->reg('a1');

    my $str = '';

    while (my $c = $vm->get_mem($straddr++, 1)) {
        $str .= $c;
    }

    printf "[Node.logstr] %s\n", $str;
}

1;
