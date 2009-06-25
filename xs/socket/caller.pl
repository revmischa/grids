




use IO::Socket::INET;

my $sock_out = IO::Socket::INET->new( PeerAddr => 'localhost',
								  PeerPort => $ARGV[0],
								  Proto => 'tcp');
$sock_out or die "no sock_out\n";

while( 1 ) {
	print "About to read...\n";
	
	my $std_read = <STDIN>;
	
	print "Read...\n";
	
	print $sock_out $std_read;
}

close( $sock );

