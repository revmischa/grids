package Grids::Test;

use strict;
use warnings;

use Grids::Node;
use Grids::Client;
use Grids::Identity;

# returns ($client, $server, $client_id, $server_id)
sub client_server_pair {
    my ($class, %opts) = @_;
    
    my $debug = $opts{debug} || 0;

    my $srv_id = Grids::Identity->create_for_test(name => 'test-server');
    my $server = Grids::Node->new(debug => $debug, id => $srv_id, transport_driver => 'Loop');
    my $server_t = $server->new_transport;

    my $id = Grids::Identity->create_for_test(name => 'test-client');
    my $client = Grids::Client->new(id => $id, transport_driver => 'Loop', debug => $debug);

    # connect client to server using Loop transport
    $client->connect($server_t);

    return ($client, $server, $id, $srv_id);
}

sub node_with_many_clients {
    my ($class, $client_count, %opts) = @_;

    my $srv_id = Grids::Identity->create_for_test(name => 'test-server');
    my $server = Grids::Node->new(id => $srv_id, %opts, transport_driver => 'Loop');
    $server->listen;

    my @clients;
    for (my $i = 0; $i < $client_count; $i++) {
	my $id = Grids::Identity->create_for_test(name => 'test-id-' . ($i + 1));
	my $client = Grids::Client->new(id => $id, transport_driver => 'Loop', %opts);

	# connect client to server using Loop transport
	$client->connect($server->new_transport);
        push @clients, $client;
    }

    return ($server, @clients);
}


1;
