use strict;
package NetTransport;

# use all subclasses
use Class::Autouse;
Class::Autouse->autouse_recursive('NetTransport');

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
