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
             t8 t9 k0 k1 gp sp fp ra hi lo
             /);
}

# make table of name => register number
{
    my $i = 0;
    %REGS = map { ($_, $i++) } @REGS;
}

# I- and J-type opcodes
our %OPCODES = (
                li      => 0b111110,
                syscall => 0b111111,
                l       => 0b111101,
                lw      => 0b100011,
                j       => 0b000010,
                addi    => 0b001000,
                addiu   => 0b001001,
                );

# definition of R-type functions
our %R_TYPE_FUNCS = (
                     add   => ["rd, rs, rt", 0b100000],
                     addu  => ["rd, rs, rt", 0b100001],
                     jr    => ["rs",         0b001000],
                     sll   => ["rd, rs, sa", 0b000000],
                     );

# syscalls
our %SYSCALLS = (
                 'Node.log'    => 32,
                 'Node.logstr' => 33,
                 );

# opcode reverse lookup table
our %SYSCALLS_REV;
@SYSCALLS_REV{values %SYSCALLS} = keys %SYSCALLS;

# handlers for special opcodes
our %SPECIAL_FUNCS = (
                      0b111110 => 'assemble_li',
                      0b111101 => 'assemble_l',
                      0b111111 => 'assemble_syscall',
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
    my $line_num = 1;
    my %labels;

    my @lines = split("\n", $asm);

    # first pass, remove labels and empty lines
    my @pass1;
    foreach my $line (@lines) {
        # remove comments
        if ((my $cmtidx = index($line, ';')) >= 0) {
            $line = substr($line, 0, $cmtidx);
        }

        # skip whitespace
        next unless $line && $line =~ /\S/;

        push @pass1, $line;
    }

    my $addr = 0;
    my @pass2;
    # second pass, calculate label offsets and parse instructions
    foreach my $line (@pass1) {
        my $err = sub {
            my $msg = shift;
            print STDERR "Error at line $line_num: $msg (\"$line\")\n";
            return 0;
        };

        if (my ($type, $data) = $line =~ /^\s*\.d([bwlsz])\s\"?([^"]+)\"?\s*$/i) { # " ) { emacs is dumb
            # data (.db .dl .dw .ds .dz)
            $type = lc $type;
                                                                 
            if ($type eq 'b') {
                # byte
                push @pass2, pack("C", $class->parse_value($data, 1));
                $addr += 1;
            } elsif ($type eq 'w') {
                # word
                push @pass2, pack("s", $class->parse_value($data, 2));
                $addr += 2;
            } elsif ($type eq 'l') {
                # long
                push @pass2, pack("l", $class->parse_value($data, 4));
                $addr += 4;
            } elsif ($type eq 's') {
                # char string
                push @pass2, $data;
                $addr += length($data);
            } elsif ($type eq 'z') {
                # null-terminated char string
                push @pass2, $data . "\0";
                $addr += length($data) + 1;
            }
        } elsif (my ($label) = $line =~ /\s*([\w\.\d]+)\s*:/) {
            # label definition
            return $err->("label $label redefined") if exists $labels{$label};
            $labels{$label} = $addr;
        } else {
            # must be an instruction. try to parse it
            my $operation; # op mnemonic
            my @pre_args; # raw arg strings

            my @instruction_regexes = (
                                       # syscall
                                       qr/
                                       ^\s*(syscall)\s*([\w.]+)\s*$
                                       /xi,

                                       # operation arg1[, arg2][, arg3]
                                       qr/
                                       ([\w.]+)\s*              # operation
                                       ([\$\d\w]+)?\s*          # 1st arg
                                       (?:,\s*([\$\d\w]+))?\s*  # 2nd
                                       (?:,\s*([\$\d\w]+))?     # 3rd
                                       /x,
                                       );

            # try each regex until one properly parses the line
            foreach my $re (@instruction_regexes) {
                ($operation, @pre_args) = $line =~ $re;
                last if $operation;
            }

            unless ($operation) {
                # did not successfully parse line
                warn "Unable to interpert line: $line\n";
            }

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
                    # convert immediate value to 32-bit data
                    my $val = $class->parse_value($arg, 32);
                    push @args, $val;
                } else {
                    if (lc $operation eq 'syscall') {
                        # syscall, dont do any special processing
                        push @args, $arg;
                    } else {
                        # label reference
                        push @args, $arg;
                    }
                }
            }

            push @pass2, [$line_num, $operation, @args];
            $addr += 6;
        }

        $line_num++;
    }

    # third pass, assemble instructions
    foreach my $inst (@pass2) {
        if (! ref $inst) {
            # just immediate data, don't need to assemble
            $ret .= $inst;
            next;
        }

        my ($line_num, $operation, @args) = @$inst;

        my $err = sub {
            my $msg = shift;
            print STDERR "Error at line $line_num: $msg (\"$operation\")\n";
            return 0;
        };

        # replace label references with calculated label offsets
        my @new_args;
        foreach my $arg (@args) {
            if ($arg =~ /\D/ && $operation ne 'syscall') {
                my $addr = $labels{$arg};
                return $err->("unknown reference to \"$arg\"") unless defined $addr;
                push @new_args, $addr;
            } else {
                push @new_args, $arg;
            }
        }

        $ret .= $class->assemble_instruction($line_num, $operation, @new_args);
    }

    return $ret;
}

# converts values such as "0x3F" and "0b11001100" to ints
sub parse_value {
    my ($class, $val, $size) = @_;
    if ($val =~ /0x([A-Fa-f0-9]+)/i) {
        # convert from hex string
        return hex($1);
    } elsif ($val =~ /0b(\d+)/i) {
        # convert from 32-bit binary string left zero padded
        my $b = $size * 8;
        return unpack("N", pack("B$b", substr("0" x $b . "$1", -32)));
    }

    return int($val);
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

# assemble syscall
sub assemble_syscall {
    my ($class, $syscall_name) = @_;

    my $syscall_num = $SYSCALLS{$syscall_name};
    die "Unknown syscall: $syscall_name\n" unless defined $syscall_num;

    my $bit_string = sprintf("%06b%032b%010b",
                             $OPCODES{syscall},
                             $syscall_num,
                             0);

    print "syscall [\"$syscall_name\"] = $bit_string\n";

    return $class->pack_bit_string($bit_string);
}

# assemble li as addi, $rt, $zero, data
sub assemble_li {
    my ($class, $rt, $data) = @_;
    return $class->assemble_i($OPCODES{addi}, $rt, 0, $data);
}    

# assemble l as add, $rd, $rs, $zero
sub assemble_l {
    my ($class, $rd, $rs) = @_;
    return $class->assemble_r('add', $rd, $rs, 0);
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

    my $bitstringlen = length($bs);
    my $bytecnt = $bitstringlen / 8;
    my @bytes = unpack("C" x $bytecnt, pack("B$bitstringlen", $bs));

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
        my @fields = qw(rt rs data);
        my @args = map { sprintf("0x%X", $fields{$_}) } @fields;
        $ret .= ' ' . join(', ', @args);
    } elsif ($type eq 'J') {
        $ret .= $class->opcode_mnemonic($opcode);
        $ret .= sprintf(" 0x%08X", $fields{data});
    } elsif ($type eq 'S') {
        my $syscall_name = $SYSCALLS_REV{$fields{syscall}};

        $ret .= $class->opcode_mnemonic($opcode);
        $ret .= sprintf(" 0x%08X", $fields{syscall});
        $ret .= " ; $syscall_name";
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
    } elsif ($type eq 'S') {
        $fields = {
            syscall => $bit_substr->(6, 32, 'N'),
        };
    }

    return undef unless $fields;

    return ($opcode, %$fields);
}

# given an opcode, returns 'R', 'I', 'J', 'S' or 'C' depending on the type
sub opcode_type {
    my ($class, $opcode) = @_;

    if (! $opcode) {
        return 'R';
    } elsif ($OPCODES_REV{$opcode} eq 'syscall') {
        return 'S';
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

