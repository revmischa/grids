#!/usr/bin/perl


#usage: socket_chat.pl receive_port send_port 

use IO::Socket::INET;
use threads;
use threads::shared;

my $continue :shared = 1;

my $sock_out;

until( $sock_out ) {
	$sock_out = IO::Socket::INET->new( PeerAddr => 'localhost',
									   PeerPort => $ARGV[1],
									   Proto => 'tcp');
}

$sock_out or die "no sock\n";
print "Writing sock created.\n";


sub listen_read {
	my( $thr_read_port ) = @_;


	my $sock_in = IO::Socket::INET->new( LocalAddr => 'localhost',
										 LocalPort => $thr_read_port,
										 Proto => 'tcp',
										 Listen => 10 );
	
	$sock_in or die "no read sock\n";
	
	my $new_sock = $sock_in->accept(); # Hangs the program here
	
	print "Reading socket created.\n";
	
	my $temp_read;

	while( $temp_read = <$new_sock> ) {
		chomp( $temp_read );
		
		print "read:\n". $temp_read . "\n";
		
	}
}

sub listen_write {
	my( $thr_write_port ) = @_;
 
}

	

#my $thr_read = threads->create( 'listen_read', $ARGV[0] );
#my $thr_write = threads->create( 'listen_write', $ARGV[1])->join();

#listen_write( $ARGV[1] );


while( 1 ) {
	my $std_read = <STDIN>;
	
	chomp( $std_read );
	
	if( $std_read eq "exit" ) {
		$continue = 0;
	}
	
	print $sock_out $std_read;
	
	print "Send $std_read\n";
}




while ( $continue > 0 ) {

}

print "Finishing\n";

$thr_read->exit();
$thr_write->exit();
