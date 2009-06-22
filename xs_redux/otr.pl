

use lib "$ENV{HOME}/perllib";

use Crypt::OTR;

Crypt::OTR::crypt_otr_set_accountname( "mrAccount" );
Crypt::OTR::crypt_otr_set_protocol( "Grids" );
Crypt::OTR::crypt_otr_set_max_message_size( 10000 );

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

