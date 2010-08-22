package Grids::Logger;

use Moose::Role;
use namespace::autoclean;

use Grids::Log;

has 'log' => (
    is => 'rw',
    isa => 'Grids::Log',
    #handles => [qw/ trace debug info warn error /],
    lazy => 1,
    builder => 'build_logger',
);

has 'log_level' => (
    is => 'rw',
    isa => 'Int',
    default => 2,
);

sub build_logger {
    my ($self) = @_;

    return Grids::Log->new(
        format_message => sub { $self->format_log_message($_[1]) },
        log_level => $self->log_level
    );
}

sub format_log_message {
    my ($self, $msg) = @_;
    return $msg;
}

1;
