use strict;
package NetTransport;

sub new {
    my ($class, $parent) = @_;

    return bless { parent => $parent }, $class;
}

sub parent { $_[0]->{parent} }

sub connection_established {
    my ($self, $con) = @_;

    NetNode::connection_established($self->parent, $self, $con);
}

1;
