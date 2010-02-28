# This is a module to define the GridsCode instruction set, handling
# assembling and disassembling GridsAsm and interperting GridsCode
package Grids::Code;

use strict;
use warnings;

use bytes;
use Class::Autouse qw/Grids::Code::Program/;

our $ASSEMBLE_BRANCH_FUNCS_SPECIAL;

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
    # pseudo opcodes
    'li'      => 0b111110,
    'la'      => 0b111001,
    'l'       => 0b111101,
    'syscall' => 0b111111,

    # based memory access
    'lb'      => 0b100000,
    'lh'      => 0b100001,
    'lw'      => 0b100011,
    'sb'      => 0b101000,
    'sh'      => 0b101001,
    'sw'      => 0b101011,

    'j'       => 0b000010,
    'jal'     => 0b000011,
    'jreli'   => 0b110001,

    # branch opcodes
    'beq'     => 0b000100,
    'bne'     => 0b000101,

    'bgez'    => 0b000001,
    'bgezal'  => 0b000111,
    'bgtz'    => 0b010101,
    'bgtzal'  => 0b010111,

    'blez'    => 0b011011,
    'blezal'  => 0b011001,
    'bltz'    => 0b011010,
    'bltzal'  => 0b010001,
    ####

    'addi'    => 0b001000,
    'addiu'   => 0b001001,
    'xori'    => 0b001110,
    'ori'     => 0b001101,
    'andi'    => 0b001100,
);

# branch opcodes
our @BRANCH_OPS = qw /
    beq bne jr bgez bgezal bgtz bgtzal bltz bltzal blez blezal
/;

our @J_TYPE_OPS = qw /j jal jreli/;

# definition of R-type functions
our %R_TYPE_FUNCS = (
    'add'   => ["rd, rs, rt", 0b100000],
    'addu'  => ["rd, rs, rt", 0b100001],
    'sub'   => ["rd, rs, rt", 0b100010],
    'subu'  => ["rd, rs, rt", 0b100011],

    'jr'    => ["rs",         0b001000],

    'sll'   => ["rd, rs, sa", 0b000000],
    'srl'   => ["rd, rs, sa", 0b000010],

    'and'   => ["rd, rs, rt", 0b100100],
    'xor'   => ["rd, rs, rt", 0b100110],
    'or'    => ["rd, rs, rt", 0b100101],
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
                      $OPCODES{li}      => 'assemble_li',
                      $OPCODES{l}       => 'assemble_l',
                      $OPCODES{la}      => 'assemble_la',
                      $OPCODES{syscall} => 'assemble_syscall',
                      );

if ($ASSEMBLE_BRANCH_FUNCS_SPECIAL) {
    # branches get assembled specially
    # (add 'assemble_branch') for all branch opcodes in %SPECIAL_FUNCS
    @SPECIAL_FUNCS{map {$OPCODES{$_}} grep { $OPCODES{$_} }@BRANCH_OPS} =
        map { 'assemble_branch' } @BRANCH_OPS;
}

# opcode reverse lookup table
our %OPCODES_REV;
@OPCODES_REV{values %OPCODES} = keys %OPCODES;

# opcodes which have an offset encoded in the immediate data
our @OFFSET_OPCODES = qw (
    lw
    lh
    lb
    sw
    sh
    sb
);

# macros
our %ALIASES = (
    'nop' => 'sll 0, 0, 0',
);

# verify opcodes on load
my %_opcodes_seen;
die "Invalid opcodes" if grep {
    ++$_opcodes_seen{$_} > 1
} values %OPCODES;

# return instruction mnemonic for opcode
sub opcode_mnemonic {
    my ($class, $op) = @_;
    return undef unless $op;
    return $OPCODES_REV{$op};
}

# FIXME: any better way to do this?
# force a number into a n-bit value
sub sn {
    my ($templ, $i, $signed) = @_;

    Carp::confess "i is undefined" unless defined $i;

    return $signed ?
        unpack($templ, pack($templ, $i)) :
        unpack(uc $templ, pack(uc $templ, $i));
}

sub s32 {
    my $i = shift;
    return sn('l', $i, 1);
}
sub u32 {
    my $i = shift;
    return sn('L', $i, 0);
}
sub s16 {
    my $i = shift;
    return sn('s', $i, 1);
}
sub u16 {
    my $i = shift;
    return sn('S', $i, 0);
}
sub s8 {
    my $i = shift;
    return sn('c', $i, 1);
}
sub u8 {
    my $i = shift;
    return sn('C', $i, 0);
}

# given a 6-byte instruction, determine the opcode
sub instruction_opcode {
    my ($class, $inst) = @_;

    my ($opcode) = $class->disassemble($inst);
    return $opcode;
}

# return instruction mnemonic for r-type function
sub r_function_mnemonic {
    my ($class, $func) = @_;

    foreach my $name (keys %R_TYPE_FUNCS) {
        return $name if $R_TYPE_FUNCS{$name}->[1] == $func;
    }

    return undef;
}

# returns if this opcode is a branch instruction
sub is_branch_opcode {
    my ($self, $opcode) = @_;
    my $mnemonic = Grids::Code->opcode_mnemonic($opcode) or return 0;
    return grep { $_ eq $mnemonic } @Grids::Code::BRANCH_OPS;
}

# returns if this R-type function is a branch instruction
sub is_branch_r_func {
    my ($self, $rfunc) = @_;
    my $mnemonic = Grids::Code->r_function_mnemonic($rfunc);
    return grep { $_ eq $mnemonic } @Grids::Code::BRANCH_OPS;
}

# takes a string of GridsAsm and returns a Program or undef if assembly was unsuccessful
sub assemble_program {
    my ($class, $asm) = @_;

    my $segment_map = $class->assemble_segment_map($asm)
        or return undef;

    return Grids::Code::Program->new(segments => $segment_map);
}

# just returns raw bytes of .text section
sub assemble_simple {
    my ($class, $asm) = @_;

    my $segmap = $class->assemble_segment_map($asm);
    return undef unless $segmap;

    # return data at base of text segment
    return $segmap->{0};
}

# takes a string of GridsAsm and returns segment map in the form { addr => bytecode }
sub assemble_segment_map {
    my ($class, $asm) = @_;

    my $ret = '';

    my $base = 'text'; # what segment are we assembling?
    my %segments = ( text => '', data => '' ); # segment => data
    my %segments_inst = ( text => [], data => [] ); # segment => instructions
    my %addr = ( text => 0, data => 0 ),

    my $line_num = 1; # current source line
    my %labels; # segment -> label -> addresses

    my @lines = split("\n", $asm);

    # first pass, remove labels and empty lines, substitute aliases
    my @pass1;
    foreach my $line (@lines) {
        # remove comments
        if ((my $cmtidx = index($line, ';')) >= 0) {
            $line = substr($line, 0, $cmtidx);
        }

        # skip whitespace
        next unless $line && $line =~ /\S/;

        # do alias substitution
        my ($cmd) = $line =~ /^\s*(\w+)\s*$/;
        my $alias = $ALIASES{lc $cmd};
        if (defined $alias) {
            $line = $alias;
        }

        # if this line begins with a label, break it into two lines
        if (my ($label, $other) = $line =~ /^\s*(\w+\s*:)\s*(.+)$/) {
            # fixme: does this break $line_num
            push @pass1, ($label, $other);
            next;
        }

        push @pass1, $line;
    }

    my @pass2;
    # second pass, calculate label offsets and parse instructions
    foreach my $line (@pass1) {
        my $err = sub {
            my $msg = shift;
            print STDERR "Error at line $line_num: $msg (\"$line\")\n";
            return 0;
        };

        if (my ($type, $data) = $line =~ /^\s*\.d([bwlsz])\s\"?([^"]+)\"?\s*$/i) { # "])/) { #emacs is dumb
            # data (.db .dl .dw .ds .dz)
            $type = lc $type;
                                                                 
            if ($type eq 'b') {
                # byte
                push @{$segments_inst{$base}}, pack("C", $class->parse_value($data, 1));
                $addr{$base} += 1;
            } elsif ($type eq 'w') {
                # word
                push @{$segments_inst{$base}}, pack("s", $class->parse_value($data, 2));
                $addr{$base} += 2;
            } elsif ($type eq 'l') {
                # long
                push @{$segments_inst{$base}}, pack("l", $class->parse_value($data, 4));
                $addr{$base} += 4;
            } elsif ($type eq 's') {
                # char string
                push @{$segments_inst{$base}}, $data;
                $addr{$base} += length($data);
            } elsif ($type eq 'z') {
                # null-terminated char string
                push @{$segments_inst{$base}}, $data . "\0";
                $addr{$base} += length($data) + 1;
            }
        } elsif (my ($directive) = $line =~ /^\s*\.(\w+)\s*$/) {
            # assembler directive
            if ($directive eq 'text') {
                $base = 'text';
            } elsif ($directive eq 'data') {
                $base = 'data';
            } else {
                return $err->("Unknown directive '.$directive'");
            }
        } elsif (my ($label) = $line =~ /\s*([\w\.\d]+)\s*:/) {
            # label definition
            return $err->("label $label redefined") if exists $labels{$base}{$label};
            $labels{$base}{$label} = $addr{$base};
        } else {
            # must be an instruction. try to parse it
            my $operation; # op mnemonic
            my @pre_args; # raw arg strings

            # rewrite offset addressing to be consistant with other
            # i-type instuction ordering
            my ($op, $rt, $offset, $rs) = $line =~ m/
                ([\w]+)\s*              # operation
                ([\-\$\d\w]+)?\s*       # 1st arg
                (?:,\s*([\-\d]+)?\(     # offset
                 \s*([\-\$\d\w]+)\s*    # 3rd arg
                 \))
            /x;
            if ($op && defined $rt && defined $rs) {
                return $err->("found indirect addressing for non-offset opcode $op")
                    unless grep { $_ eq $op } @OFFSET_OPCODES;

                $offset ||= 0;
                $line = "$op $rt, $rs, $offset";
            }

            my @instruction_regexes = (
                                       # syscall
                                       qr/
                                       ^\s*(syscall)\s*([\w.]+)\s*$
                                       /xi,

                                       # operation arg1[, arg2][, arg3]
                                       qr/
                                       ([\w.]+)\s*               # operation
                                       ([-\$\d\w]+)?\s*          # 1st arg
                                       (?:,\s*([-\$\d\w]+))?\s*  # 2nd
                                       (?:,\s*([-\$\d\w]+))?     # 3rd
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

            # process argument substitutions, indirection
            my @args;
            foreach my $arg (@pre_args) {
                next unless defined $arg;

                if ($arg =~ /^\$(\w+)/) {
                    # register name
                    my $reg_num = $REGS{lc $1};
                    return $err->("invalid register '$1'") unless defined $reg_num;
                    push @args, $reg_num;
                } elsif ($arg =~ /^\-?\d/) {
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

            push @{$segments_inst{$base}}, [$line_num, $operation, @args];
            $addr{$base} += 6;
        }

        $line_num++;
    }

    # get base address of data segment and lengths of text and data segments
    my $data_base_addr = my $text_length = $addr{text}; # data segment starts after text segment
    my $data_length = $addr{data};

    # offset all addresses in data segment by the size of the text segment
    my $data_labels = $labels{data};
    foreach my $data_label (keys %$data_labels) {
        $labels{data}{$data_label} += $data_base_addr;
    }

    # set %addr addresses to new base addresses
    $addr{data} = $data_base_addr;
    $addr{text} = 0;

    # third pass, assemble instructions
    foreach my $segment (keys %segments_inst) {
        foreach my $inst (@{$segments_inst{$segment}}) {
            if (! ref $inst) {
                # just immediate data, don't need to assemble
                $segments{$segment} .= $inst;
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
                if ($arg !~ /^[\d\-]+$/ && $operation ne 'syscall') {
                    # search for label in each segment
                    my $addr;
                    foreach my $label_seg (keys %labels) {
                        foreach my $label (keys %{$labels{$label_seg}}) {
                            if ($label eq $arg) {
                                $addr = $labels{$label_seg}->{$label};
                                last;
                            }
                        }
                        last if $addr;
                    }
                    return $err->("unknown reference to \"$arg\"") unless defined $addr;
                    push @new_args, $addr;
                } else {
                    push @new_args, $arg;
                }
            }

            $segments{$segment} ||= '';
            $segments{$segment} .= $class->assemble_instruction($line_num, $operation, \%labels, @new_args);
        }
    }

    my %ret = (map { $addr{$_} => $segments{$_} } keys %addr);
    return \%ret;
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
    my ($class, $line_num, $op, $labels, @args) = @_;

    my $err = sub {
        print STDERR sprintf("Error at line %d: %s [\"%s %s\"]\n",
                             $line_num, shift(), $op, join(', ', @args));
        return '';
    };

    my $opcode = $OPCODES{$op};

    # process assembly of special instructions
    my $assemble_func = $SPECIAL_FUNCS{$opcode} if $opcode;

    # determine operation type (r,i,j,c,s)
    my $res;

    my $type = $class->opcode_type($opcode);

    if ($assemble_func) {
        # special opcode with handler
        $res = __PACKAGE__->$assemble_func($err, $opcode, $labels, @args);
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

# assemble branch instructions
sub assemble_branch {
    my ($class, $op, @args) = @_;

    # in traditional MIPS implementations a function specifier is
    # built into $rt for certain branch instructions. this is a
    # massive pain to deal with so we are just going to assemble
    # instructions like normal for now, but this map will be here in
    # case we ever want to do this in the future
    {
        # some branch instructions have the same opcode but a different
        # function coded into $rt
        my %func_map = (
                        bgez   => 0b00001,
                        bgezal => 0b10001,
                        bgtz   => 0b00000,
                        blez   => 0b00000,
                        bltz   => 0b00000,
                        bltzal => 0b10000,
                        ); # map of opcode => $rt func code
    }

    return $class->assemble_i($op, @args);
}    

# assemble syscall
sub assemble_syscall {
    my ($class, $err, $op, $labels, $syscall_name) = @_;

    my $syscall_num = $SYSCALLS{$syscall_name};
    die "Unknown syscall: $syscall_name\n" unless defined $syscall_num;

    my $bit_string = sprintf("%06b%032b%010b",
                             $OPCODES{syscall},
                             $syscall_num,
                             0);

    print "syscall [\"$syscall_name\"] = $bit_string\n";

    return $class->pack_bit_string($bit_string);
}

# load immediate
# assemble li as addi, $rt, $zero, data
sub assemble_li {
    my ($class, $err, $op, $labels, $rt, $data) = @_;
    return $class->assemble_i($OPCODES{addi}, $rt, 0, $data);
}    

# load address
# assemble la $rt, LABEL as addi, $rt, $zero, &LABEL
sub assemble_la {
    my ($class, $err, $op, $labels, $rt, $LABEL) = @_;

    # as it happens, this opcode works exactly like "l" since the
    # labels are replaced with their addresses before this is called
    

    # find address that corresponds to LABEL
    #my $addr;
    #for my $segment (values %$labels) {
    #    $addr = $segment->{$LABEL} and last;
    #}
    #return $err->("could not find address for label $LABEL")
    #    unless $addr;


    return $class->assemble_i($OPCODES{addi}, $rt, 0, $LABEL);
}    

# load register
# assemble l as add, $rd, $rs, $zero
sub assemble_l {
    my ($class, $err, $op, $labels, $rd, $rs) = @_;
    return $class->assemble_r('add', $rd, $rs, 0);
}    

# assemble I-type instruction
sub assemble_i {
    my $class = shift;
    my $op = shift;

    my @args = @_;

    my @arg_order;

    if (scalar @args == 2) {
        # if we got two args and it is an I-type instruction, the
        # second arg is the immediate data
        @arg_order = qw /rs data/;
    } else {
        @arg_order = qw /rt rs data/;
    }

    my %fields = map { $_ => shift(@args) } @arg_order;

    $fields{data} = u32($fields{data} || 0);

    my $bit_string = sprintf("%06b", $op);

    my @inst_order = qw (rs rt data);
    foreach my $field (@inst_order) {
        my $f = $fields{$field} || 0;
        my $s = $field eq 'data' ? 32 : 5;
        $bit_string .= sprintf("%0${s}b", $f);
    }

    print "i [$op, " . join(', ', map { sprintf("0x%X", ($fields{$_} || 0)) } @inst_order) . "] = $bit_string\n";

    return $class->pack_bit_string($bit_string);
}

# assemble J-type instruction
sub assemble_j {
    my ($class, $op, $data) = @_;

    $data = u32($data);

    my $bit_string = sprintf "%06b%032b%010b", $op, $data, 0;
    printf "j [$op, 0x%08X] = $bit_string\n", $data;

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

    $op = 0; # r-type opcode is 0

    my $bit_string = sprintf "%06b%05b%05b%05b%08b%06b%013b", $op, $rs, $rt, $rd, $sa, $func, 0;
    printf "r [%06b, 0x%02X, 0x%02X, 0x%02X, %08b, %06b] = $bit_string\n", $op, $rs, $rt, $rd, $sa, $func;

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
        my $inst = $class->opcode_mnemonic($opcode);
        $ret .= $inst;
        my @fields = qw(rt rs data);
        my @args = map { sprintf("0x%X", $fields{$_}) } @fields;

        $ret .= ' ';

        if (grep { $_ eq $inst } @OFFSET_OPCODES) {
            $fields{data} ||= 0;
            $ret .= "$fields{rt}, $fields{data}($fields{rs})";
        } else {
            $ret .= join(', ', @args);
        }
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

        $template ||= "c$len";

        my $bitstr = substr($ib, $offset, $len);
        my $bitlen = $len % 4 ? $len + 4 - ($len % 4) : $len;
        $bitstr = "0" x (8 - $len % 8) . $bitstr if $len % 8;

        my $val = unpack($template, pack("B$bitlen", $bitstr));
        return $val;
    };

    my $opcode = $bit_substr->(0, 6, 'C');
    my $type = $class->opcode_type($opcode);

    my $fields;

    my $pack_template = 'N';

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
            data => $bit_substr->(16, 32, $pack_template),
        };
    } elsif ($type eq 'J') {
        $fields = {
            data => $bit_substr->(6, 32, $pack_template),
        };
    } elsif ($type eq 'S') {
        $fields = {
            syscall => $bit_substr->(6, 32, $pack_template),
        };
    } else {
        # unknown, probably data
    }

    return undef unless $fields;

    return ($opcode, %$fields);
}

# given an opcode, returns 'R', 'I', 'J', 'S' or 'C' depending on the type
sub opcode_type {
    my ($class, $opcode) = @_;

    my $mnemonic = $class->opcode_mnemonic($opcode);

    if (! $opcode) {
        # register
        return 'R';
    } elsif ($mnemonic eq 'syscall') {
        # syscall
        return 'S';
    } elsif (grep { $_ eq $mnemonic } @J_TYPE_OPS ) {
        # jump
        return 'J';
    } elsif ((($opcode >> 2) ^ 0b000100) == 0) {
        # co-processor (e.g. floating point. not yet implemented)
        return 'C';
    } else {
        # immediate data
        return 'I';
    }

    return '?'; # mystery!
}

1;

