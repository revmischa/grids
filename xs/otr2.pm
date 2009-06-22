
#!/usr/bin/perl

use warnings;


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

void cSetKeyFile( SV* sv_keyfile );
void cSetFingerprintFile( SV* sv_fprfile );
void cSetUserState( SV* sv_userstate );
void cSetID( SV* sv_id );


////////////////////////////////////
//// OTR Struct Functions
///////////////////////////////////

static OtrlPolicy cb_policy             (void *opdata, ConnContext *ctx);
static void       cb_create_privkey     (void *opdata,
								 const char *accountname,
								 const char *protocol);
static int        cb_is_logged_in       (void *opdata,
								 const char *accountname,
								 const char *protocol,
								 const char *recipient);
static void       cb_inject_message     (void *opdata,
								 const char *accountname,
								 const char *protocol,
								 const char *recipient,
								 const char *message);
static void       cb_notify             (void *opdata,
								 OtrlNotifyLevel level,
								 const char *accountname,
								 const char *protocol,
								 const char *username,
								 const char *title,
								 const char *primary,
								 const char *secondary);
static int        cb_display_otr_message(void *opdata,
								 const char *accountname,
								 const char *protocol,
								 const char *username,
								 const char *msg);
static void       cb_update_context_list(void *opdata);
static const char *cb_protocol_name     (void *opdata, const char *protocol);
static void       cb_protocol_name_free (void *opdata,
								 const char *protocol_name);
static void       cb_new_fingerprint    (void *opdata, OtrlUserState us,
								 const char *accountname,
								 const char *protocol,
								 const char *username,
								 unsigned char fingerprint[20]);
static void       cb_write_fingerprints (void *opdata);
static void       cb_gone_secure        (void *opdata, ConnContext *context);
static void       cb_gone_insecure      (void *opdata, ConnContext *context);
static void       cb_still_secure       (void *opdata, ConnContext *context,
								 int is_reply);
static void       cb_log_message        (void *opdata, const char *message);
static int        cb_max_message_size   (void *opdata, ConnContext *context);


static OtrlMessageAppOps otrOps = 
	{
		cb_policy,
		cb_create_privkey,
		cb_is_logged_in,
		cb_inject_message,
		cb_notify,
		cb_display_otr_message,
		cb_update_context_list,
		cb_protocol_name,
		cb_protocol_name_free,
		cb_new_fingerprint,
		cb_write_fingerprints,
		cb_gone_secure,
		cb_gone_insecure,
		cb_still_secure,
		cb_log_message,
		cb_max_message_size,
		NULL, /*account_name*/
		NULL  /*account_name_free*/
	};




// Initialize 
// Create a UserState, and pass it back as 2nd parameter
// Create a private key if there is not one already

int otrInit( SV * client_id, SV * userstate )
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

	printf( "temp_user_state ptr = %i\n", temp_user_state );
		
	root = expandFilename( "~/.grids/otr/" );
	
	temp_keyfile = malloc( (strlen(root) + strlen(my_id) + strlen(".key") + 1)*sizeof(char) ); // +1 for the \0
	temp_fingerprintfile = malloc( (strlen(root) + strlen( my_id) + strlen(".fpr") + 1)*sizeof(char) );
	sprintf( temp_keyfile, "%s%s.key", root, my_id);
	sprintf( temp_fingerprintfile, "%s%s.fpr", root, my_id);

	SV* sv_temp_keyfile = newSVpv( temp_keyfile, strlen( temp_keyfile ) + 1 );
	SV* sv_temp_fingerprintfile = newSVpv( temp_fingerprintfile, strlen( temp_fingerprintfile ) + 1 );
		
	cSetKeyFile( sv_temp_keyfile );
	cSetFingerprintFile( sv_temp_fingerprintfile );

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


void cSetKeyFile( SV* sv_keyfile )
{
	Inline_Stack_Vars;

	PUSHMARK(sp) ; 
	XPUSHs(sv_2mortal(newSVsv(sv_keyfile))); 
	PUTBACK; 
	
	perl_call_pv( "main::perlSetKeyFile", 0 );
}

SV* cGetKeyFile()
{
	Inline_Stack_Vars;
	
	perl_call_pv("main::perlGetKeyFile", G_SCALAR|G_NOARGS );
	SPAGAIN;
	SV* perl_keyfile = POPs;
	
	return newSVsv( perl_keyfile );
}

void cSetFingerprintFile( SV* sv_fprfile )
{
	Inline_Stack_Vars;

	PUSHMARK( sp );
	XPUSHs(sv_2mortal(newSVsv(sv_fprfile)));
	PUTBACK;
	
	perl_call_pv( "main::perlSetFingerprintFile", 0 );
}

SV* cGetFingerprintFile()
{
	Inline_Stack_Vars;
	
	perl_call_pv("main::perlGetFingerprintFile", G_SCALAR|G_NOARGS );
	SPAGAIN;
	SV* perl_fingerprintfile = POPs;
	
	return newSVsv( perl_fingerprintfile );
}

void cSetUserState( SV* sv_userstate )
{
	Inline_Stack_Vars;

	PUSHMARK( sp );
	XPUSHs(sv_2mortal(newSVsv(sv_userstate)));
	PUTBACK;
	
	perl_call_pv( "main::perlSetUserState", 0 );
}

SV* cGetUserState()
{
	Inline_Stack_Vars; // Set up stack shit

	perl_call_pv("main::perlGetUserState", G_SCALAR|G_NOARGS );
	SPAGAIN; // Adjust stack pointer for new addition
	SV* perl_user_state = POPs; // Pop it off
		
	return newSVsv( perl_user_state );
}

void cSetID( SV* sv_id )
{
	Inline_Stack_Vars;

	PUSHMARK( sp );
	XPUSHs(sv_2mortal(newSVsv(sv_id)));
	PUTBACK;
	
	perl_call_pv( "main::perlSetID", 0 );
}

SV* cGetID()
{
	Inline_Stack_Vars;
	
	perl_call_pv("main::perlGetID", G_SCALAR|G_NOARGS );
	SPAGAIN;
	SV* perl_id = POPs;
	
	return newSVsv( perl_id );
}


void otrCreatePrivKey( char* account_name, char* protocol )
{	  
	cb_create_privkey( NULL, account_name, protocol );
}



void cOtrNotify( char* accountname, char* type, char* primary, char* secondary )
{


}

void cHandleOtrMessage( char* accountname, char* username, char* msg )
{


}

cHandleOtrNewFingerprint( char* accountname, char* readable )
{


}


/////////////////////////////////////////////////////
/// Struct Methods 
////////////////////////////////////////////////////


// Return the OTR policy for the given context
static OtrlPolicy cb_policy(void *opdata, ConnContext *ctx)
{
	//options include
	// OTRL_POLICY_NEVER
	// OTRL_POLICY_OPPORTUNISTIC & ~OTRL_POLICY_ALLOW_V1

	// This is how it should be done
	// return d->getPolicyFromID( ctx->username );

	return OTRL_POLICY_OPPORTUNISTIC;
}

static void cb_create_privkey( void *opdata, const char *accountname, 
						 const char *protocol)
{
	int key_error;
	
	SV* perl_user_state = cGetUserState();	
	OtrlUserState user_state = SvIV( perl_user_state ); // extract the pointer
	printf( "Userstate extracted %i\n", user_state );
	
	SV* perl_keyfile = cGetKeyFile();
	char * keyfile = SvPV_nolen( perl_keyfile );
	printf( "Extracted keyfile: %s\n", keyfile);


	printf( "Generating new OTR key for %s.\nThis may take a while... (like several minutes srsly)\n", accountname);

	key_error = otrl_privkey_generate( user_state, keyfile, accountname, protocol );

	if( key_error ) {
		printf("***********************************\n");
		printf("OTR key generation failed!  Please make the following directory: %s\n", keyfile);		
		printf("***********************************\n");
	}
	else {
		printf("OTR key generated.\n");
	}   
}

/* Report whether you think the given user is online.  Return 1 if
 * you think he is, 0 if you think he isn't, -1 if you're not sure.
 * If you return 1, messages such as heartbeats or other
 * notifications may be sent to the user, which could result in "not
 * logged in" errors if you're wrong. */
static int        cb_is_logged_in       (void *opdata, const char *accountname, const char *protocol,
								 const char *recipient)
{
	//int ret = opt.opt_device->getComponent("utility")->idAvailable( recipient );
	return 1;
}

/* Send the given IM to the given recipient from the given
 * accountname/protocol. */
static void       cb_inject_message     (void *opdata, const char *accountname, const char *protocol,
								 const char *recipient, const char *message)
{
	/*I don't know what to put here exactly, sending messages is handled elsewhere. */

}


/* Display a notification message for a particular
 * accountname / protocol / username conversation. */
static void       cb_notify             (void *opdata, OtrlNotifyLevel level, const char *accountname,
								 const char *protocol, const char *username, const char *title,
								 const char *primary, const char *secondary)
{
	char * type;
	
	switch (level) {
	case OTRL_NOTIFY_ERROR:   type = "error";   break;
	case OTRL_NOTIFY_WARNING: type = "warning"; break;
	case OTRL_NOTIFY_INFO:    type = "info";    break;
	default:                  type = "unknown";
	}

	cOtrNotify(  accountname, type, primary, secondary );

}

/* Display an OTR control message for a particular
 * accountname / protocol / username conversation.  Return 0 if you are able
 * to successfully display it.  If you return non-0 (or if this
 * function is NULL), the control message will be displayed inline,
 * as a received message, or else by using the above notify()
 * callback. */
static int        cb_display_otr_message(void *opdata, const char *accountname, const char *protocol,
								 const char *username, const char *msg)
{
	cHandleOtrMessage( accountname, username, msg );

	return 0;
}


/* When the list of ConnContexts changes (including a change in
 * state), this is called so the UI can be updated. */
static void       cb_update_context_list(void *opdata)
{
	// lol dongs wtf?
}

/* Return a newly allocated string containing a human-friendly name
 * for the given protocol id */
static const char *cb_protocol_name     (void *opdata, const char *protocol)
{
	return protocol;
}

/* Deallocate a string allocated by protocol_name */
static void       cb_protocol_name_free (void *opdata, const char *protocol_name)
{
	//We didn't allocated memory, so we don't have to free anything
}

/* A new fingerprint for the given user has been received. */
static void       cb_new_fingerprint    (void *opdata, OtrlUserState us, const char *accountname, const char *protocol,
								 const char *username, unsigned char fingerprint[20])
{
	char readable[45];

	otrl_privkey_hash_to_human(readable, fingerprint);

	cHandleOtrNewFingerprint( accountname, readable );		
}

/* The list of known fingerprints has changed.  Write them to disk. */
static void       cb_write_fingerprints (void *opdata)
{
	SV* perl_user_state = cGetUserState();
	OtrlUserState temp_user_state = SvIV( perl_user_state ); // extract the pointer
		
	SV* perl_fprfile = cGetFingerprintFile();
	char* temp_fprfile = SvPV_nolen( perl_fprfile );
		
	otrl_privkey_write_fingerprints(temp_user_state, temp_fprfile );
}


/* A ConnContext has entered a secure state. */
static void       cb_gone_secure        (void *opdata, ConnContext *context)
{
	// Look at the members of context to see who the sender / target is,
	// then alert the appropriate objects
}

/* A ConnContext has left a secure state. */
static void       cb_gone_insecure      (void *opdata, ConnContext *context)
{

}

/* We have completed an authentication, using the D-H keys we
 * already knew.  is_reply indicates whether we initiated the AKE. */
static void       cb_still_secure       (void *opdata, ConnContext *context,
								 int is_reply)
{

}

/* Log a message.  The passed message will end in "\n". */
static void       cb_log_message        (void *opdata, const char *message)
{
	

}

/* Find the maximum message size supported by this protocol. */
static int        cb_max_message_size   (void *opdata, ConnContext *context)
{
	return 32768; // A bit of a guess, I don't suppose there's any limit really
}



////////////////////////////////////
// Utilities
///////////////////////////////////


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
	my($new_id) = @_;	
	$client_id = $new_id;
}

sub perlGetUserState {
	return $userstate;
}

sub perlSetUserState {
	my( $new_userstate ) = @_;
	$userstate = $new_userstate;
}

sub perlGetKeyFile {
	return $keyfile;	
}

sub perlSetKeyFile {
	my ($new_file) = @_;	
	print "Perl setting keyfile to: " . $new_file . "\n";	
	$keyfile = $new_file;
}

sub perlGetFingerprintFile {
	return $fingerprintfile;
}

sub perlSetFingerprintFile {
	my ($new_file) = @_;
	print "Perl setting fprfile to: " . $new_file . "\n";	
	$fingerprintfile = $new_file;
}


print "loldongs\n";


otrInit( $client_id, $userstate );


