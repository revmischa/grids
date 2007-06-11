# This is a module to define the NetCode instruction set
package NetCode;
use strict;
use Data::Dumper;

our (@REGS, %REGS); # mappings of register->symbolic name and vice-versa

# define registers
{
    @REGS = (qw/
             zero at v0 v1 a0 a1 a2 a3
             t0 t1 t2 t3 t4 t5 t6 t7
             s0 s1 s2 s3 s4 s5 s6 s7
             t8 t9 k0 k1 gp sp fp ra
             /);
}

# make table of name => register number
{
    my $i = 0;
    %REGS = map { ($_, $i++) } @REGS;
}

# I- and J-type opcodes
our %OPCODES = (
                li   => 0b111111,
                lw   => 0b100011,
                j    => 0b000010,
                addi => 0b001000,
                );

# definition of R-type functions
our %R_TYPE_FUNCS = (
                     add => ["rd, rs, rt", 0b100000],
                     );

# handlers for special opcodes
our %SPECIAL_FUNCS = (
                      0b111111 => 'assemble_li',
                      );

# opcode reverse lookup table
our %OPCODES_REV;
@OPCODES_REV{values %OPCODES} = keys %OPCODES;


# return instruction mnemonic for opcode
sub opcode_mnemonic { return $OPCODES_REV{$_[1]} }

# return instruction mnemonic for r-type function
sub r_function_mnemonic {
    my ($class, $func) = @_;

    foreach my $name (keys %R_TYPE_FUNCS) {
        return $name if $R_TYPE_FUNCS{$name}->[1] == $func;
    }

    return undef;
}

# takes a string of NetAsm and returns assembled bytecode
sub assemble {
    my ($class, $asm) = @_;

    my $ret = '';
    my $base_address = 0;
    my $pos = 0;
    my $line_num = 1;
    my %labels;

    my @lines = split("\n", $asm);
    foreach my $line (@lines) {
        my $err = sub {
            my $msg = shift;
            print STDERR "Error at line $line_num: $msg (\"$line\")\n";
            return 0;
        };

        # remove comments
        if ((my $cmtidx = index($line, ';')) >= 0) {
            $line = substr($line, 0, $cmtidx - 1);
        }

        # skip whitespace
        next unless $line && $line =~ /\S/;

        if (my ($label) = $line =~ /\s*([\w\.\d]+)\s*:/) {
            # label definition

            return $err->("label $label is invalid: label names cannot start with a number.\n")
                if $label =~ /^\d/;

            $labels{$label} = $base_address + $pos;

        } elsif (my ($operation, @pre_args) = $line =~ m/
                 ([\w.]+)\s*              # operation
                 ([\$\d\w]+)?\s*          # 1st arg
                 (?:,\s*([\$\d\w]+))?\s*  # 2nd
                 (?:,\s*([\$\d\w]+))?     # 3rd
                 /x) {
            # instruction

            # process argument substitutions
            my @args;
            foreach my $arg (@pre_args) {
                next unless $arg;

                if ($arg =~ /^\$(\w+)/) {
                    # register name
                    my $reg_num = $REGS{lc $1};
                    return $err->("invalid register '$1'") unless defined $reg_num;
                    push @args, $reg_num;
                } elsif ($arg =~ /^\d/) {
                    # immediate data

                    # convert to 32-bit data
                    my $val = int($arg);

                    if ($arg =~ /0x([A-Fa-f0-9]+)/i) {
                        # convert from hex string
                        $val = hex($1);
                    } elsif ($arg =~ /0b(\d+)/i) {
                        $val = unpack("N", pack("B32", substr("0" x 32 . "$1", -32)));
                    }

                    push @args, $val;
                } else {
                    # label reference
                    my $label_address = $labels{$arg};
                    return $err->("unknown reference to \"$arg\"") unless defined $label_address;
                    push @args, $label_address;
                }
            }

            $ret .= $class->assemble_instruction($line_num, $operation, @args);
        } else {
            warn "Unable to interpert line: $line\n";
        }

        $line_num++;
    }

    return $ret;
}

sub assemble_instruction {
    my ($class, $line_num, $op, @args) = @_;

    my $err = sub {
        print STDERR sprintf("Error at line %d: %s [\"%s %s\"]\n",
                             $line_num, shift(), $op, join(', ', @args));
        return '';
    };

    my $opcode = $OPCODES{$op}; # or return $err->("unknown instruction $op");

    # process assembly of special instructions
    my $assemble_func = $SPECIAL_FUNCS{$opcode} if $opcode;

    warn "args: @args";

    # determine operation type (r,i,j,c)
    my $res;

    my $type = $class->opcode_type($opcode);

    if ($assemble_func) {
        # special opcode with handler
        $res = __PACKAGE__->$assemble_func(@args);
    } elsif ($type eq 'R') {
        $res = $class->assemble_r($op, @args);
    } elsif ($type eq 'J') {
        $res = $class->assemble_j($opcode, @args);
    } elsif ($type eq 'C') {
        $res = $class->assemble_c($opcode, @args);
    } elsif ($type eq 'I') {
        $res = $class->assemble_i($opcode, @args);
    } else {
        die "Unknown opcode type for opcode $opcode\n";
    }

    return $res;
}

# assemble li as addi, $rt, $zero, data
sub assemble_li {
    my ($class, $rt, $data) = @_;
    return $class->assemble_i($OPCODES{addi}, $rt, 0, $data);
}    

# assemble I-type instruction
sub assemble_i {
    my ($class, $op, $rt, $rs, $data) = @_;

    my $bit_string = sprintf "%06b%05b%05b%032b", $op, $rs, $rt, $data;
    print "i [$op, $rs, $rt, $data] = $bit_string\n";

    return $class->pack_bit_string($bit_string);
}

# assemble J-type instruction
sub assemble_j {
    my ($class, $op, $data) = @_;

    my $bit_string = sprintf "%06b%032b%010b", $op, $data, 0;
    print "j [$op, $data] = $bit_string\n";

    return $class->pack_bit_string($bit_string);
}

# assemble R-type instruction
sub assemble_r {
    my ($class, $op, @args) = @_;

    # look up description of this r-type function
    my $func_desc = $R_TYPE_FUNCS{lc $op} or die "Unknown function $op\n";
    my ($fields, $func) = @$func_desc;

    my ($rs, $rt, $rd, $sa);
    $rs = $rt = $rd = $sa = 0;

    # look up which instruction fields correspond to which arguments
    foreach my $field (split(/,\s*/, $fields)) {
        my $field_val = shift @args;

        if ($field eq 'sa') {
            die "Shift amount cannot be greater than 0xFF\n" if $field_val > 0xFF;
            $sa = $field_val;
        }

        $rs = $field_val if $field eq 'rs';
        $rt = $field_val if $field eq 'rt';
        $rd = $field_val if $field eq 'rd';
    }

    my $bit_string = sprintf "%06b%05b%05b%05b%08b%06b%013b", $op, $rs, $rt, $rd, $sa, $func, 0;
    print "r [$op, $rs, $rt, $rd] = $bit_string\n";

    return $class->pack_bit_string($bit_string);
}

# return a bit string representation of an instruction packed as 6 bytes
sub pack_bit_string {
    my ($class, $bs) = @_;

    my @bytes = unpack("CCCCCC", pack("B48", $bs));

    my $ret = '';
    $ret .= pack("C", $_) foreach @bytes;
    return $ret;
}

# returns string representing dissasembly of instruction
sub disassemble_string {
    my ($class, $inst) = @_;

    my $ret = '';

    my ($opcode, %fields) = $class->disassemble($inst);
    my $type = $class->opcode_type($opcode);

    if ($type eq 'R') {
        # r-type instruction. look up desc by func field
        my $func = $fields{func};

        my $inst = $class->r_function_mnemonic($func);
        return "[Unknown R-type instruction $func]" unless $inst;

        my $params = $R_TYPE_FUNCS{$inst}->[0];
        
        $ret .= "$inst";
        my @args;
        foreach my $field (split(/,\s*/, $params)) {
            my $val = $fields{$field};

            if ($field eq 'sa') {
                push @args, $val;
                next;
            }

            push @args, '$' . $REGS[$val];
        }

        $ret .= ' ' . join(', ', @args);
    } elsif ($type eq 'I') {
        $ret .= $class->opcode_mnemonic($opcode);
        $ret .= ' ' . join(', ', values %fields);
    } elsif ($type eq 'J') {
        $ret .= $class->opcode_mnemonic($opcode);
        $ret .= sprintf(" 0x%08X", $fields{data});
    }

    return $ret;
}

# returns opcode and hash of field values
sub disassemble {
    my ($class, $inst) = @_;

    # create a bit string of the instruction to make accessing
    # bit substrings easy. h8 perl.
    my @bytes = unpack("C6", $inst);
    my $ib = '';
    $ib .= sprintf("%08b", $_) foreach @bytes;

    my $bit_substr = sub {
        my ($offset, $len, $template) = @_;
        $template ||= 'C*';
        my $bitstr = substr($ib, $offset, $len);
        $bitstr = "0" x (8 - $len % 8) . $bitstr if $len % 8;

        my $val = unpack($template, pack("B*", $bitstr));
        #printf "val ($offset, $len) [%s]: %s %b\n", substr($ib, $offset, $len), $val, $val;
        return $val;
    };

    my $opcode = $bit_substr->(0, 6);
    my $type = $class->opcode_type($opcode);

    my $fields;

    if ($type eq 'R') {
        $fields = {
            rs   => $bit_substr->(6, 5),
            rt   => $bit_substr->(11, 5),
            rd   => $bit_substr->(16, 5),
            sa   => $bit_substr->(21, 8),
            func => $bit_substr->(29, 6),
        };
    } elsif ($type eq 'I') {
        $fields = {
            rs   => $bit_substr->(6, 5),
            rt   => $bit_substr->(11, 5),
            data => $bit_substr->(16, 32, 'N'),
        };
    } elsif ($type eq 'J') {
        $fields = {
            data => $bit_substr->(6, 32, 'N'),
        };
    }

    return undef unless $fields;

    return ($opcode, %$fields);
}

# given an opcode, returns 'R', 'I', 'J' or 'C' depending on the type
sub opcode_type {
    my ($class, $opcode) = @_;

    if (! $opcode) {
        return 'R';
    } elsif ((($opcode >> 1) ^ 0b00001) == 0) {
        return 'J';
    } elsif ((($opcode >> 2) ^ 0b0100) == 0) {
        return 'C';
    } else {
        return 'I';
    }

    return '?'; # mystery!
}

1;

