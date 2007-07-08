package NetMem;

use 5.008008;
use strict;
use warnings;
use Carp qw (croak);

require Exporter;

our @ISA = qw(Exporter);

# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.

# This allows declaration	use NetMem ':all';
# If you do not need this, moving things directly into @EXPORT or @EXPORT_OK
# will save memory.
our %EXPORT_TAGS = ( 'all' => [ qw(
	
) ] );

our @EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );

our @EXPORT = qw(
	
);

our $VERSION = '0.01';

require XSLoader;
XSLoader::load('NetMem', $VERSION);

sub new {
    my ($class, $size) = @_;

    croak "No size for NetMem defined" unless $size;

    my $self = {
    };

    bless $self, $class;

    $self->init($size);

    return $self;
}

sub init {
    my ($self, $size) = @_;

    $self->{size} = $size;
    $self->{handle} = NetMem::mem_new($size);
}

sub resize {
    my ($self, $newsize) = @_;

    my $handle = $self->h or warn "null handle";
    NetMem::mem_destroy($handle);

    $self->init($newsize);
}

sub size { $_[0]->{size} }

sub set {
    my ($self, $offset, $data) = @_;
    NetMem::mem_set($self->h, $offset, $data);
}

sub get {
    my ($self, $offset, $len) = @_;
    NetMem::mem_get($self->h, $offset, $len);
}

sub h { $_[0]->{handle} }

sub DESTROY {
    my $self = shift;
    my $handle = $self->h or warn "null handle";
    NetMem::mem_destroy($handle);
}

1;
__END__

=head1 NAME

NetMem - Provide fast and reliable interface to a block of memory

=head1 SYNOPSIS

  use NetMem;

  my $mem = NetMem->new(1024 * 16); # 16 megs
  $mem->set($offset, $data);
  $mem->resize(1024 * 32);
  $mem->get($offset, $len);

=head1 DESCRIPTION

Provides a contiguous block of memory allocated for storage. Can set
and get portions of the memory.

=head2 EXPORT

None by default.

=head1 SEE ALSO

Net, Time::Cubic

=head1 AUTHOR

Mischa Spiegelmock, E<lt>mspiegelmock@gmail.comE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2007 by Mischa Spiegelmock

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.8.8 or,
at your option, any later version of Perl 5 you may have available.


=cut
