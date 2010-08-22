package Grids::Log;

use Moose;
use namespace::autoclean;

has 'log_level' => (
    is => 'rw',
    isa => 'Int',
    default => 2,
);

has 'format_message' => (
    is => 'rw',
    isa => 'Maybe[CodeRef]',
);

sub log {
    my ($self, $level, $msg) = @_;

    return if $level > $self->log_level;

    $msg = $self->format_message->($self, $msg)
        if $self->format_message;

    if ($level > 2) {
        # info/debug go to stdout
        print "$msg\n";
    } else {
        # warn/error go to stderr
        warn "$msg\n";
    }
}

sub trace {
    my ($self, $msg) = @_;
    return $self->log(5, $msg);
}

sub debug {
    my ($self, $msg) = @_;
    return $self->log(4, $msg);
}

sub info {
    my ($self, $msg) = @_;
    return $self->log(3, $msg);
}

sub warn {
    my ($self, $msg) = @_;
    return $self->log(2, $msg);
}

sub error {
    my ($self, $msg) = @_;
    return $self->log(1, $msg);
}

__PACKAGE__->meta->make_immutable;
