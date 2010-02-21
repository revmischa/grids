# this role is for classes that wish to be notified of events on a transport

package Grids::Transport::Consumer;

use Moose::Role;

requires qw/data_received configuration/;

# may also have: incoming_connection_established,
# outgoing_connection_established, connection_ready,
# connection_unready protocol_established

1;
