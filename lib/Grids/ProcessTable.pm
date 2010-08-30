# Module for managing multiple processes running in virtual machines
package Grids::ProcessTable;

use Moose;

use namespace::autoclean;
use Grids::VM;

our $PID_MAX = 65535;

has 'next_pid' => (
    is => 'rw',
    isa => 'Int',
    default => 1,
);

has 'procs' => (
    is => 'rw',
    isa => 'HashRef',
    default => sub { {} },
);

sub get_next_pid {
    my ($self) = @_;

    my $pid = $self->next_pid;
    if ($pid > $PID_MAX) {
        # find smallest unused pid
        $self->next_pid($self->min_unused_pid);
    }
    return $pid;
}

sub min_unused_pid {
    my ($self) = @_;

    for (my $i = 1; $i <= $PID_MAX; $i++) {
        if (! $self->proc($i)) {
            return $i;
        }
    }
}

sub run_program {
    my ($self, $program) = @_;

    my $pid = $self->load_program($program);
    $self->proc($pid)->run;
}

# get process by PID
sub proc {
    my ($self, $pid) = @_;
    return $self->procs->{$pid};
}

# load program into VM and give it a PID. does not execute program
# returns new process
sub load_program {
    my ($self, $program) = @_;

    # create a new VM instance for this program
    my $vm = Grids::VM->new;
    $vm->load_program($program);

    # assign PID, save it
    my $pid = $self->get_next_pid;
    my $proc = Grids::Process->new(vm => $vm, pid => $pid);
    $self->procs->{$pid} = $vm;

    return $proc;
}

__PACKAGE__->meta->make_immutable;
