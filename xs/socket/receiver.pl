

use IO::Socket::INET;



my $sock;

until( $sock ) {
	$sock = IO::Socket::INET->new( LocalAddr => 'localhost',
								   LocalPort => $ARGV[0],
								   Proto => 'tcp',
								   Listen => 10 ); # The number of queued connections
}

my $temp_read;

print "About to accept...\n";

my $new_sock = $sock->accept(); # Hangs the program here

print "Accpeted...\n";

while( $temp_read = <$new_sock> ) {
	print $temp_read;
	print "loldongs\n";
	if( $temp_read eq "exit\n" ) {
		last;
	}		
}

close( $sock );



