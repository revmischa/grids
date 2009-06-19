#!/usr/bin/perl

use warnings;

# Inline Wackyness
# 1 -- Do not include any redundant #includes of libotr, they are not needed

use Inline C => Config => 
	CC => 'gcc' =>	
	LIBS => '-L/usr/lib -lotr' =>
	INC => '-I/usr/include';

use Inline C => << 'END_OF_C_CODE';

#define context otr_context
#include <libotr/context.h>
#undef context

#include <libotr/proto.h>
#include <libotr/message.h>
#include <libotr/privkey.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROTOCOL "Grids"

char *expandFilename(const char *fname);
void otrCreatePrivKey( char* account_name, char* protocol );


static void cb_create_privkey( void *opdata, const char *accountname, 
							   const char *protocol);


// Initialize OTR
// Create a UserState, and pass it back as 2nd parameter
// Create a private key if there is not one already

int otrInit( SV * client_id, SV * userstate, SV* keyfile, SV* fingerprintfile )
{
	char* root;
	char* temp_keyfile;
	char* temp_fingerprintfile;
	char* my_id = SvPV_nolen( client_id );

	if( SvIV( userstate ) ){
		return 0;
	}

	OTRL_INIT;

	OtrlUserState temp_user_state = otrl_userstate_create();	
	sv_setiv( userstate, temp_user_state ); // this copies the integer value of the pointer into the SV (scalar value) 

	root = expandFilename( "~/.grids/otr/" );
	
	temp_keyfile = malloc( (strlen(root) + strlen(my_id) + strlen(".key") + 1)*sizeof(char) ); // +1 for the \0
	temp_fingerprintfile = malloc( (strlen(root) + strlen( my_id) + strlen(".fpr") + 1)*sizeof(char) );
	sprintf( temp_keyfile, "%s%s.key", root, my_id);
	sprintf( temp_keyfile, "%s%s.fpr", root, my_id);

	sv_setpv( keyfile, temp_keyfile );
	sv_setpv( fingerprintfile, temp_fingerprintfile );

	SV* perl_test_file = perl_call_pv("main::perlGetKeyFile", G_SCALAR|G_NOARGS);
	char* test_file = SvPV_nolen( perl_test_file );

	printf( "This is a test %s\n", test_file );

	if( otrl_privkey_read( temp_user_state, temp_keyfile ) ) {
		printf( "Could not read OTR key from %s\n", temp_keyfile);
		otrCreatePrivKey( my_id, PROTOCOL );
	}
	else {
		printf( "Loaded private key file from %s\n", temp_keyfile );
	}

	free( temp_keyfile );
	free( temp_fingerprintfile );
	
	return 0;
}

void otrCreatePrivKey( char* account_name, char* protocol )
{
	printf( "Calling cb_create for %s\n", account_name );
	cb_create_privkey( NULL, account_name, protocol );
}

static void cb_create_privkey( void *opdata, const char *accountname, 
							   const char *protocol)
{
	int key_error;

	SV* perl_user_state = perl_call_pv("main::perlGetUserState", 0);
	OtrlUserState user_state = SvIV( perl_user_state ); // extract the pointer

	printf( "Userstate extracted %i\n", user_state );

	SV* perl_keyfile = perl_call_pv("main::perlGetKeyFile", 0 );
	char * keyfile = SvPV_nolen( perl_keyfile );

	printf( "Generating new OTR key for %s.\nThis may take a while... (like several minutes srsly)", accountname);

	key_error = otrl_privkey_generate( user_state, keyfile, 
									   accountname, protocol );

	if( key_error ) {
		printf("***********************************\n");
		printf("OTR key generation failed!  Please make the following directory: %s\n", keyfile);		
		printf("***********************************\n");
	}
	else {
		printf("OTR key generated.");
	}   
}



//  expandFilename(filename)
// Expand "~/" with the $HOME env. variable in a file name.
// The caller must free the string after use.
char *expandFilename(const char *fname)
{
	char* buffer;

	if (!fname)
		return NULL;
	if (!strncmp(fname, "~/", 2)) {
		char *homedir = getenv("HOME");
		if (homedir){
			int new_size = strlen( homedir ) + strlen( fname ); // subtrat 1 for the ~, add 1 for the \0
			buffer = malloc( (new_size ) * sizeof( char ) ); 
			sprintf(buffer, "%s%s", homedir, fname + sizeof(char) ); // remove the ~		   
			return buffer;
		}
	}
	return strdup(fname);
}



END_OF_C_CODE


	package main;

# Perl code goes here
#
# Maybe all these variables should go into a hash that's passed to the functions,
# or replaced with one function, perlGetOtrHash, that would give all the variables


$client_id = "mrcodyrose"; #GridsID, etc should go here
$userstate = 0; # Set the "pointer" to NULL
$keyfile = "";
$fingerprintfile = "";

sub perlGetID {
	return $client_id;
}

sub perlSetID {

}

sub perlGetUserState {
	return $userstate;
}

sub perlSetUserState {

}

sub perlGetKeyFile {
	return $keyfile;
}

sub perlSetKeyFile {

}

sub perlGetFingerprintFile {
	return $fingerprintfile;
}

sub perlSetFingerprintFile {

}


print "loldongs\n";

#print "Fingerprint = " .  perlGetFingerprintFile() . "\n";

otrInit( $client_id, $userstate, $keyfile, $fingerprintfile );

print "perl\n";
print $keyfile . "\n";
