# This class represents a peer that is connected to a Node. It can be
# a client or another Node.

package NetNode::Remote;
use strict;
use warnings;
use base qw/Class::Accessor::Fast/;

__PACKAGE__->mk_accessors(qw/trans public_key session_token/);

sub new {
    my ($class, %opts) = @_;

    my $self = {
        %opts,
    };

    return bless $self, $class;
}

sub id { $_[0]->public_key }

1;
