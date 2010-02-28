# This module describes the actual implementation of the instructions
# in the GridsVM

package Grids::VM::Instructions;

use strict;
use warnings;
use Grids::Code;

*dbg = \&Grids::VM::dbg;

### utility funcs

# returns 32-bit unsigned representation of this number
sub _ul {
    my $i = shift;
    return Grids::Code::u32($i);
}
# signed version
sub _sl {
    my $i = shift;
    return Grids::Code::s32($i);
}
sub _uw {
    my $i = shift;
    return Grids::Code::u16($i);
}
sub _sw {
    my $i = shift;
    return Grids::Code::s16($i);
}
sub _ub {
    my $i = shift;
    return Grids::Code::u8($i);
}
sub _sb {
    my $i = shift;
    return Grids::Code::s8($i);
}

#sub _u { return int(sprintf("%u", $_[0])) }
#sub _u { return unpack('L', pack('J', $_[0])) }
#sub _s { return int(sprintf("%ld", $_[0])) }
#sub _s { return $_[0] }
#sub _s { return unpack("l", pack('j', $_[0])) }
#sub _s { return pack("l", unpack('l', $_[0])) }


# return 32-bit bit string
sub _bs { return sprintf("%032b", $_[0]) }

### instructions

# long jump, takes 32-bit address
sub j_j {
    my ($class, $vm, $address) = @_;

    $vm->{pc} = _ul($address);
}

# long jump and link, takes 32-bit address
sub j_jal {
    my ($class, $vm, $address) = @_;

    $vm->link;
    $vm->{pc} = _ul($address);
}

# offset pc by $data
sub j_jreli {
    my ($class, $vm, $addr) = @_;
    $vm->{pc} += _sl($addr) * 6;
}

# rd = rs + rt
sub r_add {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    $vm->set_reg($rd, $vm->reg($rs) + $vm->reg($rt));
}

# rd = unsigned(rs) + unsigned(rt)
sub r_addu {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    $vm->set_reg($rd, $vm->reg_u($rs) + $vm->reg_u($rt));
}

# rd = rs - rt
sub r_sub {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    $vm->set_reg($rd, $vm->reg($rs) - $vm->reg($rt));
}

# rd = unsigned(rs)? - unsigned(rt)
sub r_subu {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    $vm->set_reg($rd, $vm->reg_u($rs) - $vm->reg_u($rt));
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
    $vm->set_reg($rd, $vm->regs->xor($rs, $rt));
}

# rt = $rs + $data
sub i_addi {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_reg($rt, $vm->reg($rs) + _sl($data));
}

# rt = $rs ^ $data
sub i_xori {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_reg($rt, $vm->reg_u($rs) ^ _ul($data));
}

# rt = $rs | $data
sub i_ori {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_reg($rt, $vm->reg($rs) | _ul($data));
}

# rd = rs & rt
sub r_and {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    $vm->set_reg($rd, $vm->regs->and($rs, $rt));
}

# rd = rs | rt
sub r_or {
    my ($class, $vm, $rs, $rt, $rd, $sa) = @_;
    $vm->set_reg($rd, $vm->regs->or($rs, $rt));
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
    my $mem_val = $vm->get_mem_u($vm->reg_u($rs) + _sl($data), 2);
    $vm->set_reg($rt, $mem_val);
}

# $data($rs) = rt
sub i_sw {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_mem($vm->reg_u($rs) + $data, _sw($vm->reg($rt)), 2);
}

# $data($rs) = rt
sub i_sb {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_mem($vm->reg_u($rs) + $data, $vm->reg($rt), 1);
}

# rt = $data($rs) - 1 byte
sub i_lb {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_reg($rt, $vm->get_mem($vm->reg_u($rs) + _sb($data), 1));
}

# is $rs unsigned too? need to check
# rt = unsigned($rs) + unsigned($data)
sub i_addiu {
    my ($class, $vm, $rs, $rt, $data) = @_;
    $vm->set_reg($rt, _ul($vm->reg_u($rs) + _ul($data)));
}

my %branch_funcs = (

                    # if $rs == $rt pc = data; else advance_pc (6);
                    beq => sub {
                        my $f = shift;
                        $f->{rs} == $f->{rt} ? $f->{data} : undef;
                    },

                    # if $rs != $rt pc = data; else advance_pc (6);
                    bne => sub {
                        my $f = shift;
                        $f->{rs} != $f->{rt} ? $f->{data} : undef;
                    },

                    # if $rs >= 0 pc = $data; else advance_pc (6);
                    bgez => sub {
                        my $f = shift;
                        $f->{rs} >= 0 ? $f->{data} : undef;
                    },

                    # if $rs > 0 pc = data; else advance_pc (6);
                    bgtz =>  sub {
                        my $f = shift;
                        $f->{rs} > 0 ? $f->{data} : undef;
                    },

                    # if $rs <= 0 pc = $data; else advance_pc (6);
                    blez => sub {
                        my $f = shift;
                        $f->{rs} <= 0 ? $f->{data} : undef;
                    },

                    # if $rs < 0 pc = data; else advance_pc (6);
                    bltz =>  sub {
                        my $f = shift;
                        $f->{rs} < 0 ? $f->{data} : undef;
                    },

                    # pc = $rs
                    jr => sub {
                        my $f = shift;
                        $f->{rs};
                    },

                    );

sub branch {
    my ($class, $vm, $func, $fields) = @_;

    # is this a "and link" branch?
    my $is_link = $func =~ s/al$//i;

    my $brfunc = $branch_funcs{$func} or die "Unknown branch function $func";

    # lookup register values
    $fields->{$_} = $fields->{$_} ? $vm->reg($fields->{$_}) : $fields->{$_} for qw/ rs rt rd /;
    my $res = $brfunc->($fields);

    if (defined $res) {
        # branching

        # link ($ra = PC + 6) if we are linking
        $vm->link if $is_link;

        return $res;
    }

    # don't branch
    return undef;
}

1;
