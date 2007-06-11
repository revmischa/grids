# This module describes the actual implementation of the instructions
# in the NetVM

use strict;
package NetVM::Instructions;

*dbg = \&NetVM::dbg;

# long jump, takes 32-bit address
sub j_j {
    my ($class, $vm, $address) = @_;

    $vm->{pc} = $address;
    #dbg(sprintf("set pc to %032b\n", $address));
}

# rd = rs + rt
sub r_add {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    $vm->set_reg($rd, $vm->reg($rs) + $vm->reg($rt));
}

# rt = $rs + $data
sub i_addi {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_reg($rt, $vm->reg($rs) + $data);
}

1;
