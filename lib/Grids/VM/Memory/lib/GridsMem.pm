package Grids::VM::Mem;

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

# This allows declaration	use Grids::VM::Mem ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.
our %EXPORT_TAGS = ( 'all' => [ qw(
	
) ] );

our @EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );

our @EXPORT = qw(
	
);

our $VERSION = '0.01';

require XSLoader;
XSLoader::load('Grids::VM::Mem', $VERSION);

sub new {
    my ($class, $size) = @_;

    croak "No size for Grids::VM::Mem defined" unless $size;

    my $self = {
    };

    bless $self, $class;

    $self->init($size);

    return $self;
}

sub size {
    my ($self, $size) = @_;
    return $self->{size} unless defined $size;
    croak "Invalid size $size" if $size < 0;
    $self->{size} = $size;
}

sub init {
    my ($self, $size) = @_;

    $self->size($size);
    $self->{handle} = Grids::VM::Mem::mem_new($size);
}

sub resize {
    my ($self, $newsize, %opts) = @_;

    my $oldsize = $self->size;
    my $oldhandle = $self->h or warn "null handle";

    $self->init($newsize);
    my $copysize = $oldsize > $newsize ? $newsize : $oldsize;

    if ($oldsize && $copysize && ! $opts{nocopy}) {
        Grids::VM::Mem::mem_copy($oldhandle, $self->h, $copysize);
    }

    Grids::VM::Mem::mem_destroy($oldhandle);
}

sub set {
    my ($self, $offset, $data) = @_;

    croak "Tried to set memory outside bounds"
        if $offset + length $data > $self->size;

    Grids::VM::Mem::mem_set($self->h, $offset, $data);
}

sub get {
    my ($self, $offset, $len) = @_;

    $offset ||= 0;
    $len = $self->size unless defined $len;

    croak "Tried to get memory outside bounds"
        if $offset + $len > $self->size;

    Grids::VM::Mem::mem_get($self->h, $offset, $len);
}

# resize to new data size and set to new data
sub load {
    my ($self, $data) = @_;
    my $size = length $data;
    $self->resize($size, nocopy => 1);
    $self->set(0, $data);
}

sub h { $_[0]->{handle} }

sub DESTROY {
    my $self = shift;
    my $handle = $self->h or warn "null handle";
    Grids::VM::Mem::mem_destroy($handle);
}

1;
__END__

=head1 NAME

Grids::VM::Mem - Provide a fast and reliable interface to a block of memory

=head1 SYNOPSIS

  use Grids::VM::Mem;

  my $mem = Grids::VM::Mem->new(1024 * 16); # 16 megs
  $mem->set($offset, $data);
  $mem->resize(1024 * 32);
  $mem->get($offset, $len);

=head1 DESCRIPTION

Provides a contiguous block of memory allocated for storage. Can set
and get portions of the memory.

=head2 EXPORT

None by default.

=head1 SEE ALSO

Grids

=head1 AUTHOR

Mischa Spiegelmock, E<lt>mspiegelmock@gmail.comE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2008 by Mischa Spiegelmock

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.8.8 or,
at your option, any later version of Perl 5 you may have available.


=cut
