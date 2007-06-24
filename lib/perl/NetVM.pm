package NetVM;
use strict;
use Carp qw (croak);
use MemHandle;
use IO::Seekable;
use Data::Dumper;
use NetCode;
use NetVM::Instructions;
use Class::Autouse;

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

    my $self = {
        mem_limit => $mem_limit,
        regs      => [],
    };

    bless $self, $class;
    $self->init;

    return $self;
}

# initializes the vm
sub init {
    my $self = shift;

    # initialize registers
    $self->{regs}->[$_] = 0 for (0..31);

    # empty memory
    $self->{mem} = new MemHandle;

    # reset PC
    $self->{pc} = 0;
}

=item regs()

Returns arrayref of registers 0-31

=cut

sub regs { $_[0]->{regs} }


=item reg($reg)

Returns contents of register C<$reg>, where C<$reg> is either the register
number or the register name (e.g. 8 or 't0')

=cut

sub reg {
    my ($self, $reg) = @_;

    my $reg_idx = $self->_reg($reg);
    return $self->regs->[$reg_idx];
}


=item set_reg($reg, $val)

Set contents of register C<$reg> to the 32-bit integer value in $val

=cut

sub set_reg {
    my ($self, $reg, $val) = @_;

    my $reg_idx = $self->_reg($reg);

    croak "Cannot modify zero register" if $reg_idx == 0;

    return $self->regs->[$reg_idx] = $val;
}

# utility method to look up a register number that accepts either the
# register number (e.g. 4) or a name (e.g. 'a0')
sub _reg {
    my ($self, $reg) = @_;

    return undef unless $reg;

    my $reg_num = $REGS{lc $reg} || $reg;

    croak "Invalid register $reg" if $reg_num < 0 || $reg_num > 31;

    return $reg_num
}

# read part of memory
sub get_mem {
    my ($self, $offset, $len) = @_;

    $len ||= 1; # default to one byte

    # this is really slow and dumb. there is probably a way
    # to make this a lot faster.
    my $mem = $self->mem;
    my $contents = '';
    $mem->seek($offset, SEEK_SET);
    $mem->sysread($contents, $len);

    return $contents;
}

sub mem_size {
    my ($self) = @_;

    my $mem = $self->mem;
    $mem->seek(0, SEEK_END);
    return $mem->tell;
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

sub pc { $_[0]->{pc} }


sub mem { $_[0]->{mem} }


=item load($bytecode)

Loads C<$bytecode> at 0x00000000

=cut

sub load {
    my ($self, $bytecode) = @_;

    # TODO: check memory constraints

    # seek to 0x00000000
    my $mem = $self->mem;
    $mem->seek(0, SEEK_SET);

    $mem->mem($bytecode);
    return;
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
    $self->mem->seek($self->pc, SEEK_SET);
    my $inst;
    my $read = read($self->mem, $inst, 6);

    unless ($read) {
        # memory EOF, do nothing?
        return undef;
    }

    return $inst;
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

    return 0 if $self->{pc} >= $self->mem_size;

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

    my $func = 'i_' . NetCode->opcode_mnemonic($opcode)
        or die "Unknown instruction $opcode";

    my @args;
    push @args, $fields{$_} for qw(rs rt data);

    my $handler_package = __PACKAGE__ . "::Instructions";
    "$handler_package"->$func($self, @args);

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

    my $func = 'r_' . NetCode->r_function_mnemonic($fields{func})
        or die "Unknown r-type function $fields{func}";

    my @args;
    push @args, $fields{$_} for qw(rs rt rd sa);

    my $handler_package = __PACKAGE__ . "::Instructions";
    "$handler_package"->$func($self, @args);

    # increment PC
    $self->{pc} += 6;
}

sub dbg {
    my ($msg) = @_;

    return unless $DEBUG;
    print $msg . "\n";
}

1;
