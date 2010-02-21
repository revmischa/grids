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
    my $server = Grids::Node->new(debug => $debug, id => $srv_id);
    my $server_trans = $server->add_transport('Loop');

    my $id = Grids::Identity->create_for_test(name => 'test-client');
    my $client = Grids::Client->new(id => $id, transport_class => 'Loop', debug => $debug);

    # connect client to server using Loop transport
    $client->connect($server_trans);

    return ($client, $server, $id, $srv_id);
}

sub node_with_many_clients {
    my ($class, $client_count, %opts) = @_;

    my $srv_id = Grids::Identity->create_for_test(name => 'test-server');
    my $server = Grids::Node->new(id => $srv_id, %opts);

    my @clients;
    for (my $i = 0; $i < $client_count; $i++) {
	my $id = Grids::Identity->create_for_test(name => 'test-id-' . ($i + 1));
	my $client = Grids::Client->new(id => $id, transport_class => 'Loop', %opts);

	# connect client to server using Loop transport
        my $server_trans = $server->add_transport('Loop');
	$client->connect($server_trans);
        push @clients, $client;
    }

    return ($server, @clients);
}


1;
