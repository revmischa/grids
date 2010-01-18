package Grids::VM::Register;

use 5.008008;
use strict;
use warnings;
use Carp qw (croak);
use bytes;

our $VERSION = '0.01';

require XSLoader;
XSLoader::load('Grids::VM::Register', $VERSION);

sub new {
    my ($class, $size) = @_;

    croak "No size for Grids::VM::Register defined" unless $size;

    my $self = {};
    bless $self, $class;

    $self->init($size);

    return $self;
}

sub init {
    my ($self, $size) = @_;

    $self->{size} = $size;
    $self->{handle} = Grids::VM::Register::alloc_regs($size);
}

sub size { $_[0]->{size} }

sub set {
    my ($self, $index, $data) = @_;
    croak "Trying to set register at index $index > " . $self->size
        if $index > $self->size;
    Grids::VM::Register::set_reg($self->h, $index, $data);
}

*set_u = \&set_unsigned;
sub set_unsigned {
    my ($self, $index, $data) = @_;
    croak "Trying to set register at index $index > " . $self->size
        if $index > $self->size;
    Grids::VM::Register::set_reg_u($self->h, $index, $data);
}

sub get {
    my ($self, $index) = @_;
    croak "Trying to get register at index $index > " . $self->size
        if $index > $self->size;
    return Grids::VM::Register::get_reg($self->h, $index);
}

*get_u = \&get_unsigned;
sub get_unsigned {
    my ($self, $index) = @_;
    croak "Trying to get register at index $index > " . $self->size
        if $index > $self->size;
    return Grids::VM::Register::get_reg_u($self->h, $index);
}

sub and {
    my ($self, $idx1, $idx2) = @_;
    return Grids::VM::Register::and_regs($self->h, $idx1, $idx2);
}

sub or {
    my ($self, $idx1, $idx2) = @_;
    return Grids::VM::Register::or_regs($self->h, $idx1, $idx2);
}

sub xor {
    my ($self, $idx1, $idx2) = @_;
    return Grids::VM::Register::xor_regs($self->h, $idx1, $idx2);
}

sub h { $_[0]->{handle} }

sub DESTROY {
    my $self = shift;
    my $handle = $self->h or croak "null handle";
    Grids::VM::Register::dealloc_regs($handle);
}

1;
__END__

=head1 NAME

Grids::VM::Register - Provide fast and reliable interface to a set of 32-bit registers

=head1 SYNOPSIS

  use Grids::VM::Register;

  my $regs = Grids::VM::Registers->new(16); # create 16 32-bit registers
  $mem->set(3, 0xFFFFFFFF); # set register at index 4 to a 32-bit value
  $mem->get(3); # retrieve register value
  $mem->and(3, 6); # 0b00000110

=head1 DESCRIPTION

This is a very simple module that simply allows you to store and
retrieve 32-bit values in a set of registers.

=head2 EXPORT

None by default.

=head1 SEE ALSO

Grids

=head1 AUTHOR

Mischa Spiegelmock, E<lt>mspiegelmock@gmail.comE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2010 by Mischa Spiegelmock

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.8.8 or,
at your option, any later version of Perl 5 you may have available.


=cut
