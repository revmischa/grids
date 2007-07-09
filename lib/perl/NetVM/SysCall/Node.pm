use strict;
package NetVM::SysCall::Node;

# prints value in $a0
sub log {
    my ($vm) = @_;


    printf("[Node.log] %032b %s %u 0x%08X\n",
           $vm->reg('a0'), $vm->reg('a0'), $vm->reg('a0'), $vm->reg('a0'));
}

# prints null-terminated string at addr in $a0
sub logstr {
    my ($vm) = @_;

    my $straddr = $vm->reg('a0');

    my $str = '';

    while (my $c = $vm->get_mem($straddr++, 1)) {
        $str .= chr($c);
    }

    printf "[Node.logstr] %s\n", $str;
}

1;
