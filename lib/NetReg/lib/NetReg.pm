package NetReg;

use 5.008008;
use strict;
use warnings;
use Carp qw (croak);
use bytes;

require Exporter;

our @ISA = qw(Exporter);

# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.

# This allows declaration	use NetReg ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.
our %EXPORT_TAGS = ( 'all' => [ qw(
	
) ] );

our @EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );

our @EXPORT = qw(
	
);

our $VERSION = '0.01';

require XSLoader;
XSLoader::load('NetReg', $VERSION);

sub new {
    my ($class, $size) = @_;

    croak "No size for NetReg defined" unless $size;

    my $self = {};
    bless $self, $class;

    $self->init($size);

    return $self;
}

sub init {
    my ($self, $size) = @_;

    $self->{size} = $size;
    $self->{handle} = NetReg::alloc_regs($size);
}

sub size { $_[0]->{size} }

sub set {
    my ($self, $index, $data) = @_;
    NetReg::set_reg($self->h, $index, $data);
}

*set_u = \&set_unsigned;
sub set_unsigned {
    my ($self, $index, $data) = @_;
    NetReg::set_reg_u($self->h, $index, $data);
}

sub get {
    my ($self, $index) = @_;
    return NetReg::get_reg($self->h, $index);
}

*get_u = \&get_unsigned;
sub get_unsigned {
    my ($self, $index) = @_;
    return NetReg::get_reg_u($self->h, $index);
}

sub and {
    my ($self, $idx1, $idx2) = @_;
    return NetReg::and_regs($self->h, $idx1, $idx2);
}

sub or {
    my ($self, $idx1, $idx2) = @_;
    return NetReg::or_regs($self->h, $idx1, $idx2);
}

sub xor {
    my ($self, $idx1, $idx2) = @_;
    return NetReg::xor_regs($self->h, $idx1, $idx2);
}

sub h { $_[0]->{handle} }

sub DESTROY {
    my $self = shift;
    my $handle = $self->h or croak "null handle";
    NetReg::dealloc_regs($handle);
}

1;
__END__

=head1 NAME

NetReg - Provide fast and reliable interface to a set of 32-bit registers

=head1 SYNOPSIS

  use NetReg;

  my $regs = NetRegs->new(16); # create 16 32-bit registers
  $mem->set(3, 0xFFFFFFFF); # set register at index 4 to a 32-bit value
  $mem->get(3); # retrieve register value
  $mem->and(3, 6); # 0b00000110

=head1 DESCRIPTION

This is a very simple module that simply allows you to store and
retrieve 32-bit values in a set of registers.

=head2 EXPORT

None by default.

=head1 SEE ALSO

Net

=head1 AUTHOR

Mischa Spiegelmock, E<lt>mspiegelmock@gmail.comE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2007 by Mischa Spiegelmock

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.8.8 or,
at your option, any later version of Perl 5 you may have available.


=cut
