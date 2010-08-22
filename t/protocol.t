use strict;
use Test::More qw(no_plan);
use lib 'lib';
use Grids::Protocol;
use Grids::Identity;
use Grids::Protocol::Connection;
use Grids::Transport::Null;

my %EVENTS; # holds event info received in Protocol event handler callback

# create some identities
my $cli_id = Grids::Identity->create_for_test(name => 'proto-test-1');
my $srv_id = Grids::Identity->create_for_test(name => 'proto-test-2');

my $p_cli;
test_json();
test_protobuf();

sub test_json {
    $p_cli = Grids::Protocol->new(
        serializer_class => 'JSON',
        id => $cli_id,
    );
    run_tests();
}

sub test_protobuf {
    $p_cli = Grids::Protocol->new(
        serializer_class => 'ProtocolBuffer',
        id => $cli_id,
    );
    run_tests();
}

sub run_tests {
    my $initiation = $p_cli->initiation_string;

    my $loop = new Grids::Transport::Null;
    my $srv_conn = new Grids::Protocol::Connection(transport => $loop, channel => 1, inbound => 0);
    my $p_srv = Grids::Protocol->new_from_initiation_string($initiation, $srv_conn, { id => $srv_id });
    $srv_conn->protocol($p_srv);

    ok($p_srv, "Created new Grids::Protocol from initiation string");

    # test serialization
    {
        my $req = $p_cli->serialize_event('Echo', {
            echo_string => '123456',
        });

        my $evt = $p_srv->deserialize_event($req);

        is($evt->name, 'Echo', 'Correct event type');
        is($evt->echo_string, '123456', 'Correct event args');
    }
}
