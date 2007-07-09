# This module describes the actual implementation of the instructions
# in the NetVM

use strict;
package NetVM::Instructions;

*dbg = \&NetVM::dbg;

### utility funcs

# returns 32-bit unsigned representation of this number
sub _u { return int(sprintf("%u", $_[0])) }
sub _s { return int(sprintf("%d", $_[0])) }
# signed version
#sub _s { return int(sprintf("%ld", $_[0])) }
# return 32-bit bit string
sub _bs { return sprintf("%032b", $_[0]) }

### instructions

# long jump, takes 32-bit address
sub j_j {
    my ($class, $vm, $address) = @_;

    $vm->{pc} = _u($address);
}

# jump to address in $rs
sub r_jr {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    $vm->{pc} = $vm->reg($rs);
}

# rd = rs + rt
sub r_add {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    $vm->set_reg($rd, $vm->reg($rs) + $vm->reg($rt));
}

# rd = unsigned(rs) + unsigned(rt)
sub r_addu {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    $vm->set_reg($rd, _u($vm->reg($rs)) + _u($vm->reg($rt)));
}

# rd = rs << sa
sub r_sll {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;

    # if $rd $rs and $sa are all 0, this is "sll 0, 0, 0" which
    # really means nop
    return if ! $rs && ! $rt && ! $sa;

    $vm->set_reg($rd, $vm->reg($rs) << $sa);
}

# rd = rs >> sa
sub r_srl {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    $vm->set_reg($rd, $vm->reg($rs) >> $sa);
}

# rd = rs ^ rt
sub r_xor {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    $vm->set_reg($rd, $vm->reg($rs) ^ $vm->reg($rt));
}

# rt = $rs + $data
sub i_addi {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_reg($rt, $vm->reg($rs) + _s($data));
}

# rt = $rs ^ $data
sub i_xori {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_reg($rt, $vm->reg($rs) ^ _u($data));
}

# rt = $rs | $data
sub i_ori {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_reg($rt, $vm->reg($rs) | _u($data));
}

# rd = rs & rt
sub r_and {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    my $res = (_bs($vm->reg($rs)) & _bs($vm->reg($rt)));
    $res = unpack("N", pack("B32", $res));
   
    $vm->set_reg($rd, $res);
}

# rt = $rs & $data
sub i_andi {
    my ($class, $vm, $rs, $rt, $data) = @_;

    my $res = (_bs($vm->reg($rs)) & _bs($data));
    $res = unpack("N", pack("B32", $res));
    $vm->set_reg($rt, $res);
}

# rt = $data($rs)
sub i_lw {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_reg($rt, $vm->get_mem($vm->reg($rs) + $data, 4));
}

# rt = $data($rs) - 1 byte
sub i_lb {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_reg($rt, $vm->get_mem($vm->reg($rs) + $data, 1));
}

# is $rs unsigned too? need to check
# rt = unsigned($rs) + unsigned($data)
sub i_addiu {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_reg($rt, _u($vm->reg($rs)) + _u($data));
}

1;
