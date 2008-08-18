use strict;
use warnings;

package Grids::Address;

use Carp qw/croak/;

use base qw/Class::Accessor/;

__PACKAGE__->mk_accessors(qw/address port transport/);

sub new {
    my ($class, %opts) = @_;

    my $transport = $class->def_transport || $opts{transport} or croak "No transport defined";
    my $address = $opts{address} or croak "No address defined";
    my $port = $opts{port};

    $port ||= $class->port_required if $class->port_required;

    my $self = {
        transport => $transport,
        address => $address,
        port => $port,
    };

    return bless $self, $class;
}

sub stringify {
    my $self = shift;
    return join('.', ($self->transport || 'undef'),
                $self->address,
                ($self->port || 'undef'));
}

# override in subclasses
sub def_transport { undef }
sub port_required { undef }

1;
