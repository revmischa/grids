




use IO::Socket::INET;
use threads;
use threads::shared;



my $sock_out;


sub listen_read {
	my( $thr_read_port ) = @_;


	my $sock_in = IO::Socket::INET->new( LocalAddr => 'localhost',
										 LocalPort => $thr_read_port,
										 Proto => 'tcp',
										 Listen => 10 );
	
	$sock_in or die "no sock_in\n";
	
	my $new_sock = $sock_in->accept(); # Hangs the program here
	
	print "Reading socket created.\n";
	
	my $temp_read;

	while( $temp_read = <$new_sock> ) {
		chomp( $temp_read );
		
		print "read:\n". $temp_read . "\n";
		
	}
}

my $thr_read = threads->create( 'listen_read', $ARGV[0] );

until( $sock_out ) {
	$sock_out = IO::Socket::INET->new( PeerAddr => 'localhost',
									   PeerPort => $ARGV[1],
									   Proto => 'tcp');
}

$sock_out or die "no sock_out\n";


while( 1 ) {
	print "About to read...\n";
	
	my $std_read = <STDIN>;
	
	print "Read...\n";
	
	print $sock_out $std_read;
}

close( $sock );

