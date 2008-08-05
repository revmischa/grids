# This package represents an executable GridsCode-based Program

package Grids::Code::Program;
use strict;
use warnings;
use bytes;

use Data::Dumper;
use GridsMem;
use Storable;
use Carp;
use base qw/ Class::Accessor /;

__PACKAGE__->mk_accessors(qw/ segments /);

sub new {
    my ($class, %opts) = @_;

    my $segments = delete $opts{segments} || {};

    my $self = {
        segments => $segments,
    };

    bless $self, $class;
    return $self;
}

# returns the raw bytes representing the Program
sub serialize {
    my ($self) = @_;

    return Storable::nfreeze($self->segments);
}

# returns a byte array of a memory image with the segments mapped
sub bytes {
    my ($self) = @_;

    my $segment_map = $self->segments
        or croak "No segments";

    # calculate total length of program
    my $prog_len = 0;
    foreach my $base_addr (keys %$segment_map) {
        $prog_len = $base_addr if $base_addr > $prog_len;
        $prog_len += length $segment_map->{$base_addr};
    }

    # load a memory image with the segments mapped
    my $mem = GridsMem->new($prog_len);
    foreach my $base_addr (keys %$segment_map) {
        $mem->set($base_addr, $segment_map->{$base_addr}, offset => $base_addr);
    }

    # return bytes contained in the memory image
    return $mem->get;
}

sub load_from_file {
    my ($class, $filename) = @_;

    my $file;
    local $/;
    open $file, $filename or croak "Could not read file $filename: $!";
    my $contents = <$file>;
    close $file;

    my $segments = Storable::thaw($contents)
        or return undef;

    return $class->new(segments => $segments);

    return $segments;
}

1;
