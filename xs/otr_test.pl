#!/usr/bin/perl

# Notes on process
# 1 -- Establish OTR with user, sending any generated inject messages to user
# 2 -- Take message you want to send, pass in to crypt_otr_process_sending
# 3 -- The second parameter will either be NULL, or an encrypted message
# 4 -- If the message is encrypted, send it to the other person
# 5 -- The other person takes this encrypted message, and passes it to crypt_otr_process_receiving
# 6 -- Again, the second parameter will either be decrypted, or not, if not, then you probably have not properly established a connection
# 7 -- Person 2 composes a reply, passes it to crypt_otr_process_sending, and if the second parameter is modefied, they should send the encrypted message to person 1
# 8 -- Person 1 passes the encrypted message to crypt_otr_process_receiving, and the second parameter should be decrypted

# Other Notes
# 1 -- OTR will generate messages that are used for its own internal communication, therefore it is necessary to send messages from the callback inject_cb, and the receiver should 
#      pass the data to crypt_otr_process_receiving
# 2 -- Threading.  The only barriers to threading are changes made to OtrlUserState, and the contexts.  This plugin is essentially a C program with hooks into your running 
#      perl program.  In short, at present it does not seem possible to reliably incorporate perl threads into the C code.  

# Notes on use
# still in progess

use warnings;
use lib "$ENV{HOME}/perllib";

use Crypt::OTR;

package main;

$accountname = $ARGV[0];
$username = $ARGV[1];
print "username  " .  $username . "\n";

$write_file = '>' . $accountname . '.txt';
$read_file = '<' . $username . '.txt';
$msg_receive = "";
$msg_receive_out = "lolhi";
$secure_connection = 1;
$my_message_send;

print "readfile  " . $read_file . "\n";


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




sub write_to_file {
	my( $msg ) = @_;

	print "Writing to file $write_file, press any key to continue...\n";
	print $msg . "\n";
	<STDIN>;
	
	open (MYFILE, $write_file );
	print MYFILE $msg;
	close (MYFILE);
}

sub read_from_file{
	print "Reading from file $read_file, pressany key to continue...\n";
	<STDIN>;
	
	open( MYFILE, $read_file);
		
	@raw_data = <MYFILE>;
	$big_msg = "";
	
	foreach $line_var (@raw_data) {
		$big_msg = $big_msg . $line_var;
	}
	
	close(MYFILE);
	
	print "Read from file $read_file the following, press any key...\n";
	print  $big_msg . "\n";
	
	
	print "Read end\n";
	return $big_msg;
}

sub perl_inject_message {
	my( $actname, $proto, $them, $msg ) = @_;
	
	printf( "perl_inject\naccount: %s -- proto: %s -- them: %s -- msg: %s\n", $actname, $proto, $them, $msg );
	
	write_to_file( $msg );
}

sub perl_display_message {
	my( $actname, $proto, $them, $msg ) = @_;
	
	printf( "perl_display -- account: %s -- proto: %s -- them: %s -- msg: %s\n", $actname, $proto, $them, $msg );
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
	
	$secure_connection = 1;
}

sub perl_unverified {
	my( $connected_user ) = @_;
	
	printf( "**********\nUnverified Connection with %s started\n**********\n", $connected_user );

	$secure_connection = 1;
}




Crypt::OTR::crypt_otr_init( );

Crypt::OTR::crypt_otr_establish( $username );

while( 1 ) {
	$msg_receive = read_from_file(); 
	
	print "Decrypting the following message, press any key to continue...\n";
	print $msg_receive . "\n";
	<STDIN>;
	
	#print "While loop before:\n" . $msg_receive . "\n";
	$msg_receive_out = Crypt::OTR::crypt_otr_process_receiving( $username, $msg_receive );
	#print "while loop after:\n" . $msg_receive_out . "\n";	
	
	print "Decrypted the following message, press any key to continue...\n";
	print $msg_receive_out . "\n";
	<STDIN>;
	
	if( $secure_connection > 0) {
		print "Send  message message, type in message to continue...\n";		
		$my_message_send = <STDIN>;

		chomp( $my_message_send );

		$my_message_send_encrypt = Crypt::OTR::crypt_otr_process_sending( $username, $my_message_send );
		
		write_to_file( $my_message_send_encrypt );
	}
	
}
