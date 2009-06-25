



use warnings;
use lib "$ENV{HOME}/perllib";

use Crypt::OTR;


use IO::Socket::INET;
use threads;
use threads::shared;

package main;

my $sock_out;

$accountname = $ARGV[2];
$username = $ARGV[3];
print "username  " .  $username . "\n";

$write_file = '>' . $accountname . '.txt';
$read_file = '<' . $username . '.txt';
$msg_receive = "";
$msg_receive_out = "lolhi";
$secure_connection = 1;
$my_message_send;

Crypt::OTR::crypt_otr_set_accountname( $accountname );
Crypt::OTR::crypt_otr_set_protocol( "Grids" );
Crypt::OTR::crypt_otr_set_max_message_size( 10000 );
Crypt::OTR::crypt_otr_set_root( "~/.grids/otr/" ); # default is ~/.otr

#These two functions are the most important.  
Crypt::OTR::crypt_otr_set_inject_cb( "main::perl_inject_message" );
Crypt::OTR::crypt_otr_set_display_cb( "main::perl_display_message" );

Crypt::OTR::crypt_otr_set_display_cb( "main::perl_notify_error" );
Crypt::OTR::crypt_otr_set_display_cb( "main::perl_notify_warning" );
Crypt::OTR::crypt_otr_set_display_cb( "main::perl_notify_info" );

Crypt::OTR::crypt_otr_set_connected_cb( "main::perl_connected" );
Crypt::OTR::crypt_otr_set_unverified_cb( "main::perl_unverified" );



sub perl_inject_message {
	my( $actname, $proto, $them, $msg ) = @_;
	
	printf( "perl_inject\naccount: %s -- proto: %s -- them: %s -- msg: %s\n", $actname, $proto, $them, $msg );
	
	print $sock_out $msg;
}

sub perl_display_message {
	my( $actname, $proto, $them, $msg ) = @_;
	
	printf( "perl_display -- account: %s -- proto: %s -- them: %s -- msg:\n%s\n", $actname, $proto, $them, $msg );
}

sub perl_notify_error {
	my( $account, $proto, $user, $title, $primary, $secondary ) = @_;
	
	printf( "perl_notify_error\n%s\n%s\n%s\n%s\n%s\n%s\n", $account, $proto, $user, $title, $primary, $secondary );
}

sub perl_notify_warning {
	my( $account, $proto, $user, $title, $primary, $secondary ) = @_;
	
	printf( "perl_notify_warning\n%s\n%s\n%s\n%s\n%s\n%s\n", $account, $proto, $user, $title, $primary, $secondary );
}

sub perl_notify_info {
	my( $account, $proto, $user, $title, $primary, $secondary ) = @_;
	
	printf( "perl_notify_info\n%s\n%s\n%s\n%s\n%s\n%s\n", $account, $proto, $user, $title, $primary, $secondary );
}

sub perl_connected {
	my( $connected_user ) = @_;
	
	printf( "**********\nConnection with %s started\n**********\n", $connected_user );
}

sub perl_unverified {
	my( $connected_user ) = @_;
	
	printf( "**********\nUnverified Connection with %s started\n**********\n", $connected_user );
}




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
		#chomp( $temp_read );
		
		#print "read:\n". $temp_read . "\n";
		
		handle_receive( $temp_read );		
	}
}

sub handle_receive {
	my( $in_msg ) = @_;
	
	$msg_receive_out = Crypt::OTR::crypt_otr_process_receiving( $username, $in_msg );

	print "Decrypted the following message:\n";
	print $msg_receive_out;
	print "\n";
}


my $thr_read = threads->create( 'listen_read', $ARGV[0] );

until( $sock_out ) {
	$sock_out = IO::Socket::INET->new( PeerAddr => 'localhost',
									   PeerPort => $ARGV[1],
									   Proto => 'tcp');
}

$sock_out or die "no sock_out\n";

Crypt::OTR::crypt_otr_init( );

#Crypt::OTR::crypt_otr_establish( $username );

perl_inject_message( "one", "two", "three", "lol hi" );


while( 1 ) {
	print "About to read...\n";
	
	my $std_read = <STDIN>;
	
	print "Read...\n";
	
	print $sock_out $std_read;
}


close( $sock );

