package NetVM;
use strict;
use bytes;

use Carp qw (croak);
use Class::Autouse;
use Data::Dumper;

use NetCode;
use NetVM::Instructions;
use NetMem;
use NetReg;

use base qw (Class::Accessor::Fast);
__PACKAGE__->mk_accessors(qw/mem pc mem_limit/);

Class::Autouse->autouse_recursive('NetVM::SysCall');

our $DEBUG = 1;

=head1 NAME NetVM - A VM instance in an object

=head1 SYNOPSIS

=head1 DESCRIPTION

blah blah

=cut

    our @REGS = @NetCode::REGS;
    our %REGS = %NetCode::REGS;


=item new

Returns a new C<NetVM>, ready to load and execute C<NetCode> bytecode.

=cut

sub new {
    my ($class, %opts) = @_;

    my $mem_limit = delete $opts{memory_limit} || 128;

    croak "Unknown options: " . join(',', keys %opts) if %opts;

    my $regs = new NetReg(scalar @REGS)
        or die "couldn't create register storage";

    my $self = {
        mem_limit => $mem_limit,
        regs      => $regs,
    };

    bless $self, $class;
    $self->init;

    return $self;
}

# initializes the vm
sub init {
    my $self =  shift;

    croak "invalid vm" unless $self->{regs};

    # initialize zero register
    $self->regs->set(0, 0);

    # empty memory
    $self->{mem} = new NetMem(1);

    # reset PC
    $self->{pc} = 0;
}

=item regs()

Returns NetReg object containing registers

=cut

sub regs { $_[0]->{regs} }

=item reg($reg)

Returns contents of register C<$reg>, where C<$reg> is either the register
number or the register name (e.g. 8 or 't0')

=cut

sub reg {
    my ($self, $reg) = @_;
    my $reg_idx = $self->_reg($reg);
    return $self->regs->get($reg_idx);
}

sub reg_u {
    my ($self, $reg) = @_;
    my $reg_idx = $self->_reg($reg);
    return $self->regs->get_u($reg_idx);
}

=item set_reg($reg, $val)

Set contents of register C<$reg> to the 32-bit integer value in $val

=cut

sub set_reg {
    my ($self, $reg, $val) = @_;

    my $reg_idx = $self->_reg($reg);
    croak "undefined register" unless defined $reg_idx;

    croak "Cannot modify zero register" if $reg_idx == 0;

    return $self->regs->set($reg_idx, $val);
}

# utility method to look up a register number that accepts either the
# register number (e.g. 4) or a name (e.g. 'a0')
sub _reg {
    my ($self, $reg) = @_;

    return undef unless defined $reg;

    my $reg_num = $REGS{lc $reg} || $reg;

    croak "Invalid register $reg" if $reg_num < 0 || $reg_num > 31;

    return $reg_num;
}

# read part of memory
sub get_mem {
    my ($self, $offset, $len) = @_;
    $len ||= 1; # default to one byte

    my $contents = $self->mem->get($offset, $len);

    return unpack("c", $contents) if $len == 1;

    my @bytes = unpack("c$len", $contents);
    return join('', @bytes); # is this right?
}

sub mem_size {
    my ($self) = @_;

    return $self->mem->size;
}

=item reg_name($reg)

Returns symbolic name of register $reg

=cut

sub reg_name {
    my ($self, $reg) = @_;

    return $REGS[$reg];
}


=item pc()

Returns current program counter

=cut

=item load($bytecode)

Loads C<$bytecode> at 0x00000000

=cut

sub load {
    my ($self, $bytecode) = @_;

    $self->mem->resize(length $bytecode);
    $self->mem->set(0, $bytecode);

    return 1;
}

=item link()

Sets $ra to PC + 6

=cut

sub link {
    my ($self) = @_;
    $self->set_reg('ra', $self->pc + 6);
}

=item step()

Steps the VM one instruction.
Returns true if success, undef if no instruction executed.

=cut

sub step {
    my ($self) = @_;

    my $inst = $self->current_instruction;

    # execute instruction
    return $self->execute($inst);
}

# returns instruction at PC
sub current_instruction {
    my $self = shift;

    # read 6-byte instruction at PC
    my $inst = $self->mem->get($self->pc, 6);

    return $inst;
}

# returns opcode of current instruction
sub current_instruction_opcode {
    my $self = shift;
    my $inst = $self->current_instruction;
    return NetCode->instruction_opcode($inst);
}

# returns opcode of current instruction
sub current_instruction_r_func {
    my $self = shift;
    my $inst = $self->current_instruction;
    my ($opcode, %fields) = NetCode->disassemble($inst);
    return $fields{func};
}

sub execute {
    my ($self, $inst) = @_;

    my ($opcode, %fields) = NetCode->disassemble($inst);
    my $type = NetCode->opcode_type($opcode);

    if ($type eq 'R') {
        $self->execute_r(%fields);
    } elsif ($type eq 'I') {
        $self->execute_i($opcode, %fields);
    } elsif ($type eq 'J') {
        $self->execute_j($opcode, %fields);
    } elsif ($type eq 'S') {
        $self->syscall($fields{syscall});
        $self->{pc} += 6;
    } else {
        die "Unknown instruction type '$type'\n";
    }

    return 0 if $self->pc >= $self->mem_size;

    return 1;
}

# do a syscall
sub syscall {
    my ($self, $syscall) = @_;

    # lookup syscall name
    my $sysc_name = $NetCode::SYSCALLS_REV{$syscall};
    die "Unknown syscall $syscall" unless defined $sysc_name;

    # convert syscall to method call
    $sysc_name =~ s/\./::/g;

    no strict 'refs';
    my $method_name = "NetVM::SysCall::$sysc_name";
    $method_name->($self);
}

sub execute_i {
    my ($self, $opcode, %fields) = @_;

    my $func = NetCode->opcode_mnemonic($opcode)
        or die "Unknown instruction $opcode";

    $func = "i_$func";

    my @args;
    push @args, $fields{$_} for qw(rs rt data);

    my $handler_package = __PACKAGE__ . "::Instructions";
    my $res = "$handler_package"->$func($self, @args);

    if (NetCode->is_branch_opcode($opcode)) {
        # this is a branch I-type instruction.
        # don't touch PC if it returns true.
        return if $res;
    }

    # increment PC
    $self->{pc} += 6;
}

sub execute_j {
    my ($self, $opcode, %fields) = @_;

    my $addr = $fields{data};

    my $func = 'j_' . NetCode->opcode_mnemonic($opcode)
        or die "Unknown instruction $opcode";

    my $handler_package = __PACKAGE__ . "::Instructions";
    "$handler_package"->$func($self, $addr);
}

sub execute_r {
    my ($self, %fields) = @_;

    my $rfunc = $fields{func};

    my $func = 'r_' . NetCode->r_function_mnemonic($rfunc)
        or die "Unknown r-type function $fields{func}";

    my @args;
    push @args, $fields{$_} for qw(rs rt rd sa);

    my $handler_package = __PACKAGE__ . "::Instructions";
    my $res = "$handler_package"->$func($self, @args);

    if (NetCode->is_branch_r_func($rfunc)) {
        # this is a branch I-type instruction.
        # don't touch PC if it returns true.
        return if $res;
    }

    # increment PC
    $self->{pc} += 6;
}

sub dbg {
    my ($msg) = @_;

    return unless $DEBUG;
    print $msg . "\n";
}

1;
