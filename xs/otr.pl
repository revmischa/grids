

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



use lib "$ENV{HOME}/perllib";

use Crypt::OTR;

Crypt::OTR::crypt_otr_set_accountname( "mrAccount" );
Crypt::OTR::crypt_otr_set_protocol( "Grids" );
Crypt::OTR::crypt_otr_set_max_message_size( 10000 );

#These two functions are the most important.  
Crypt::OTR::crypt_otr_set_inject_cb( "main::perl_inject_message" );
Crypt::OTR::crypt_otr_set_display_cb( "main::perl_display_message" );

Crypt::OTR::crypt_otr_set_display_cb( "main::perl_notify_error" );
Crypt::OTR::crypt_otr_set_display_cb( "main::perl_notify_warning" );
Crypt::OTR::crypt_otr_set_display_cb( "main::perl_notify_info" );


package main;

sub perl_inject_message {
	my( $actname, $proto, $them, $msg ) = @_;
	
	printf( "perl_inject\naccount: %s\nproto: %s\nthem: %s\nmsg: %s\n", $actname, $proto, $them, $msg );
}

sub perl_display_message {
	my( $actname, $proto, $them, $msg ) = @_;
	
	printf( "perl_display\naccount: %s\nproto: %s\nthem: %s\nmsg: %s\n", $actname, $proto, $them, $msg );
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


$username = "mrPatrick";

Crypt::OTR::crypt_otr_init( );

Crypt::OTR::crypt_otr_establish( $username );

