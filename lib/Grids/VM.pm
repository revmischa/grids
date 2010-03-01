package Grids::VM;

use Moose;

use bytes;

use Carp qw/ croak /;
use Class::Autouse;
use Data::Dumper;
use Scalar::Util qw / weaken /;

use Grids::Code;
use Grids::VM::Instructions;

use Grids::VM::Memory;
use Grids::VM::Register;

has mem => (
    is => 'rw',
    isa => 'Grids::VM::Memory',
    lazy => 1,
    builder => 'build_mem',
);

has regs => (
    is => 'rw',
    isa => 'Grids::VM::Register',
    lazy => 1,
    builder => 'build_regs',
);

has pc => (
    is => 'rw',
    isa => 'Int',
);

has show_debug => (
    is => 'rw',
    isa => 'Bool',
    default => sub { 0 },
);

has show_warnings => (
    is => 'rw',
    isa => 'Bool',
    default => sub { 1 },
);

# client/node
has base => (
    is => 'rw',
    does => 'Grids::Base',
    required => 0,
);

has memory_limit => (
    is => 'rw',
    isa => 'Int',
    default => sub { 128 * 1024 * 1024 },
);

has syscall_modules => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

has syscall_handlers => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

=head1 NAME

Grids::VM

=head1 SYNOPSIS

A Grids::VM instance in an object

=head1 DESCRIPTION

blah blah

=cut

    our @REGS = @Grids::Code::REGS;
    our %REGS = %Grids::Code::REGS;

=head1 METHODS

=over 4

=item new

Returns a new L<Grids::VM>, ready to load and execute L<GridsCode> bytecode.

=cut

sub build_regs {
    my $self = shift;
    
    my $regs = new Grids::VM::Register(scalar @REGS)
        or die "couldn't create register storage";

    return $regs;
}

sub BUILD {
    my $self = shift;
    $self->init;
    $self->load_syscall_modules;
}

=item init

Resets the VM. Clears registers and memory.

=cut

sub init {
    my $self =  shift;

    # empty memory
    $self->mem(new Grids::VM::Memory(1));

    $self->init_regs;
}

=item init_regs

Reset registers including PC, doesn't clear memory

=cut

sub init_regs {
    my ($self) = @_;

    croak "invalid vm" unless $self->regs;

    # initialize zero register
    $self->regs->set(0, 0);

    # reset PC
    $self->pc(0);
}    

=item load_syscall_modules

Load all configured system call modules

=cut

sub load_syscall_modules {
    my ($self) = @_;

    foreach my $module (qw/Debug POSIX/) {
        $self->load_syscall_module($module);
    }
}

=item load_syscall_module($module)

Loads system call handlers for Grids::VM::SysCall::$module

=cut
sub load_syscall_module {
    my ($self, $module) = @_;

    my $module_class = $module;
    $module_class =~ s/[^-_\w\:]//g;

    unless ($module_class) {
        warn "tried to load malformed syscall module $module";
        next;
    }

    $module_class = "Grids::VM::SysCall::$module_class";

    unless (eval "use $module_class; 1;") {
        warn "Error loading SysCall module $module: $@";
        next;
    }

    $self->syscall_modules->{lc $module} = $module_class->new(
        vm => $self,
    );
}

=item register_syscall($syscall, $cb)

Set a callback function to be called when $syscall is executed. Only
one handler may be defined for each syscall.

=cut
sub register_syscall {
    my ($self, $syscall, $cb) = @_;

    if ($self->syscall_handlers->{$syscall}) {
        croak "System call $syscall already has a registered handler";
    }

    # save callback
    $self->syscall_handlers->{$syscall} = $cb;
}

=item base

Returns the Node/Client this VM is attached to

=cut

=item regs

Returns Grids::Reg object containing registers

=cut

=item reg($reg)

Returns contents of register C<$reg>, where C<$reg> is either the register
number or the register name (e.g. 8 or 't0')

=cut

sub reg {
    my ($self, $reg) = @_;
    my $reg_idx = $self->_reg($reg);
    return $self->regs->get($reg_idx);
}

=item reg_u($reg)

Like C<reg($reg)> but returns unsigned value

=cut

sub reg_u {
    my ($self, $reg) = @_;
    my $reg_idx = $self->_reg($reg);
    return $self->regs->get_u($reg_idx);
}

=item set_reg($reg, $val)

Set contents of register C<$reg> to the 32-bit integer value in C<$val>

=cut

sub set_reg {
    my ($self, $reg, $val) = @_;

    my $reg_idx = $self->_reg($reg);
    croak "undefined register" unless defined $reg_idx;

    croak "Cannot modify zero register" if $reg_idx == 0;

    return $self->regs->set($reg_idx, $val);
}

sub set_hilo {
    my ($self, $hi, $lo) = @_;

    my $hi_idx = $self->_reg('hi');
    my $lo_idx = $self->_reg('lo');

    $self->regs->set($hi_idx, $hi);
    $self->regs->set($lo_idx, $lo);
}

sub set_hilo_u {
    my ($self, $hi, $lo) = @_;

    my $hi_idx = $self->_reg('hi');
    my $lo_idx = $self->_reg('lo');

    $self->regs->set_u($hi_idx, $hi);
    $self->regs->set_u($lo_idx, $lo);
}

# utility method to look up a register number that accepts either the
# register number (e.g. 4) or a name (e.g. 'a0')
sub _reg {
    my ($self, $reg) = @_;

    return undef unless defined $reg;

    my $reg_num = $REGS{lc $reg} || $reg;

    croak "Invalid register $reg" if $reg_num < 0 || $reg_num > @REGS;

    return $reg_num;
}

=item get_mem($offset, $len)

Returns string of bytes of length C<$len> at offset C<$offset>

=cut

sub get_mem {
    my ($self, $offset, $len, $unsigned) = @_;
    $len ||= 1; # default to one byte

    my $contents = $self->mem->get($offset, $len);

    my $map = $unsigned ? {
        1 => 'C',
        2 => 'W',
        4 => 'L',
        8 => 'Q',
    } : {
        1 => 'c',
        2 => 'w',
        4 => 'l',
        8 => 'q',
    };

    my $template = $map->{$len};
    $template ||= "a$len";

    return unpack($template, $contents);
}

sub get_mem_u {
    my ($self, $offset, $len) = @_;
    return $self->get_mem($offset, $len, 1);
}

=item set_mem($offset, $val, $len)

Sets C<$offset> to C<$val> as C<$len> bytes long

=cut

sub set_mem {
    my ($self, $offset, $val, $len) = @_;
    $len ||= 1; # default to one byte

    my $store_val;

    my $template = {
        1 => 'c',
        2 => 'w',
        4 => 'l',
        8 => 'q',
    }->{$len} || "a$len";

    $store_val = pack($template, $val);
    $self->mem->set($offset, $store_val, $len);
}

=item mem_size

Returns size of memory

=cut

sub mem_size {
    my ($self) = @_;

    return $self->mem->size;
}

=item reg_name($reg)

Returns symbolic name of register C<$reg>

=cut

sub reg_name {
    my ($self, $reg) = @_;

    return $REGS[$reg];
}

=item pc

Returns current program counter

=cut

=item load($bytecode[, %opts])

Loads C<$bytecode> at C<$opts{offset}> || 0x00000000 and resizes memory unless C<$opts{no_resize}>

=cut

sub load {
    my ($self, $bytecode, %opts) = @_;

    my $offset = delete $opts{offset} || 0;

    croak "Invalid offset $offset" if $offset < 0;
    return 0 unless $bytecode;

    my $mem_max = length($bytecode) + $offset;

    croak "code is too large at $mem_max bytes. memory limit is " .
        ($self->memory_limit / 1024 / 1024) . "MB" if $mem_max > $self->memory_limit;

    $self->mem->resize($mem_max) unless $opts{no_resize};
    $self->mem->set($offset, $bytecode);

    return 1;
}

=item load_segments(\%segment_map)

Loads C<\%segment_map> as (address => data)

=cut

sub load_segments {
    my ($self, $segment_map) = @_;

    # calculate total length of program
    my $prog_len = 0;
    foreach my $base_addr (keys %$segment_map) {
        $prog_len = $base_addr if $base_addr > $prog_len;
        $prog_len += length $segment_map->{$base_addr};
    }

    croak "segement map is too large at $prog_len bytes. memory limit is " .
        ($self->memory_limit / 1024 / 1024) . "MB" if $prog_len > $self->memory_limit;

    $self->mem->resize($prog_len);

    foreach my $base_addr (keys %$segment_map) {
        $self->load($segment_map->{$base_addr}, offset => $base_addr, no_resize => 1);
    }
}

=item load_program($program)

Loads Program C<$program>

=cut

sub load_program {
    my ($self, $program) = @_;
    return $self->load_segments($program->segments);

    # reset PC
    $self->pc(0);
}

=item link

Sets $ra to PC + 6

=cut

sub link {
    my ($self) = @_;
    $self->set_reg('ra', $self->pc + 6);
}

=item step

Steps the VM one instruction.
Returns true if success, undef if no instruction executed.

=cut

sub step {
    my ($self) = @_;

    my $inst = $self->current_instruction;

    # execute instruction
    return $self->execute($inst);
}

=item current_instruction

Returns instruction at PC

=cut

sub current_instruction {
    my $self = shift;

    # read 6-byte instruction at PC
    my $inst = $self->mem->get($self->pc, 6);

    return $inst;
}

=item current_instruction_opcode

Returns opcode of current instruction

=cut

sub current_instruction_opcode {
    my $self = shift;
    my $inst = $self->current_instruction;
    return Grids::Code->instruction_opcode($inst);
}

=item current_instruction_opcode

Returns R-type function of current instruction

=cut

sub current_instruction_r_func {
    my $self = shift;
    my $inst = $self->current_instruction;
    my ($opcode, %fields) = Grids::Code->disassemble($inst);
    return $fields{func};
}

=item execute($inst)

Executes instruction $inst

=cut

sub execute {
    my ($self, $inst) = @_;

    my ($opcode, %fields) = Grids::Code->disassemble($inst);
    my $type = Grids::Code->opcode_type($opcode);

    # is this a branch opcode?
    if (Grids::Code->is_branch_opcode($opcode)) {
        # handle branching
        $self->execute_branch($opcode, \%fields);
    } elsif ($type eq 'R' && Grids::Code->is_branch_r_func($fields{func})) {
        $self->execute_branch_r(\%fields);
    } else {
        if ($type eq 'R') {
            $self->execute_r(\%fields);
        } elsif ($type eq 'I') {
            $self->execute_i($opcode, \%fields);
        } elsif ($type eq 'J') {
            $self->execute_j($opcode, \%fields);
        } else {
            die "Unknown instruction type '$type'\n";
        }

        return 0 if $self->pc >= $self->mem_size;
    }

    return 1;
}

=item syscall($syscall)

Does a system call

=cut

sub syscall {
    my ($self, $syscall) = @_;

    # lookup syscall name
    my $sysc_func = $self->syscall_handlers->{$syscall};
    die "Unknown syscall $syscall" unless defined $sysc_func;
    $sysc_func->($self);
}

=item execute_branch($opcode, \%fields)

Executes a branch instruction

=cut

sub execute_branch {
    my ($self, $opcode, $fields) = @_;

    my $func = Grids::Code->opcode_mnemonic($opcode)
        or die "Unknown branch instruction $opcode";

    $self->_execute_branch($func, $fields);
}

=item execute_branch_r(\%fields)

Executes an R-type branch instruction (e.g. jr)

=cut

sub execute_branch_r {
    my ($self, $fields) = @_;

    my $rfunc = $fields->{func};

    my $func = Grids::Code->r_function_mnemonic($rfunc)
        or die "Unknown r-type branch function $fields->{func}";

    $self->_execute_branch($func, $fields);
}

sub _execute_branch {
    my ($self, $func, $fields) = @_;

    my $handler_package = __PACKAGE__ . "::Instructions";
    my $res = "$handler_package"->branch($self, $func, $fields);

    if (defined $res) {
        # branch function returned a location to branch to
        $self->pc($res);
    } else {
        # branch function returned undef, which means advance PC like
        # normal (do not branch)
        $self->pc($self->pc + 6);
    }
}

=item execute_i($opcode, \%fields)

Executes an I-type instruction

=cut

sub execute_i {
    my ($self, $opcode, $fields) = @_;

    my $func = Grids::Code->opcode_mnemonic($opcode)
        or die "Unknown instruction $opcode";

    my $i_sub = "i_$func";
    my $handler_package = __PACKAGE__ . "::Instructions";

    die "Undefined I-type handler $i_sub"
        unless $handler_package->can($i_sub);

    my @args;
    push @args, $fields->{$_} for qw(rs rt data);

    "$handler_package"->$i_sub($self, @args);

    # increment PC
    $self->pc($self->pc + 6);
}

=item execute_j($opcode, \%fields)

Executes an J-type instruction

=cut

sub execute_j {
    my ($self, $opcode, $fields) = @_;

    my $addr = $fields->{data};

    my $func = 'j_' . Grids::Code->opcode_mnemonic($opcode)
        or die "Unknown instruction $opcode";

    my $handler_package = __PACKAGE__ . "::Instructions";
    "$handler_package"->$func($self, $addr);
}

=item execute_r(\%fields)

Executes an R-type instruction

=cut

sub execute_r {
    my ($self, $fields) = @_;

    my $rfunc = $fields->{func};

    my $func = 'r_' . Grids::Code->r_function_mnemonic($rfunc)
        or die "Unknown r-type function $fields->{func}";

    my @args;
    push @args, $fields->{$_} for qw(rs rt rd sa);

    my $handler_package = __PACKAGE__ . "::Instructions";
    "$handler_package"->$func($self, @args);

    # increment PC
    $self->pc($self->pc + 6);
}

=item dbg($msg)

Prints debug message C<$msg>

=cut
sub dbg {
    my ($self, $msg) = @_;

    return unless $self->show_debug;
    print $msg . "\n";
}

=item wrn($msg)

Prints warning message C<$msg>

=cut
sub wrn {
    my ($self, $msg) = @_;

    return unless $self->show_warnings;
    print STDERR $msg . "\n";
}

=back

=cut

no Moose;
__PACKAGE__->meta->make_immutable;

