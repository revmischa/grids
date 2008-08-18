use strict;
use warnings;

package Grids::Peer;

use Carp qw/croak/;

use base qw/Class::Accessor/;

__PACKAGE__->mk_accessors(qw/address id/);

sub new {
    my ($class, %opts) = @_;

    my $self = {
        address => $opts{address},
        id => $opts{id},
    };

    return bless $self, $class;
}

sub stringify {
    my $self = shift;
    return join(',', ($self->address || 'undef'), ($self->id || 'undef'));
}

1;
