package Grids::Test;

use strict;
use warnings;

use Grids::Node;
use Grids::Client;
use Grids::Identity;

our %default_opts = (
    transport_driver => 'Loop',
    log_level => 3,
);

# returns ($client, $server, $client_id, $server_id)
sub client_server_pair {
    my ($class, %opts) = @_;
    
    my $srv_id = Grids::Identity->create_for_test(name => 'test-server');
    my $server = Grids::Node->new(id => $srv_id, %default_opts, %opts);
    my $server_t = $server->new_transport;

    my $id = Grids::Identity->create_for_test(name => 'test-client');
    my $client = Grids::Client->new(id => $id, %default_opts, %opts);

    return ($client, $server, $id, $srv_id, $server_t);
}

sub node_with_many_clients {
    my ($class, $client_count, %opts) = @_;

    my $srv_id = Grids::Identity->create_for_test(name => 'test-server');
    my $server = Grids::Node->new(id => $srv_id, %opts, %default_opts);
    $server->listen;

    my @clients;
    for (my $i = 0; $i < $client_count; $i++) {
	my $id = Grids::Identity->create_for_test(name => 'test-id-' . ($i + 1));
	my $client = Grids::Client->new(id => $id, %default_opts, %opts);

	# connect client to server using Loop transport
	$client->connect($server->new_transport);
        push @clients, $client;
    }

    return ($server, @clients);
}


1;
