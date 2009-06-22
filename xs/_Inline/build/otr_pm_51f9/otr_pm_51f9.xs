#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"
#include "INLINE.h"

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
#define ACCOUNT "GridsAccount"

char *expandFilename(const char *fname);
void otrCreatePrivKey( char* account_name, char* protocol );
ConnContext* otrGetContext( SV* sv_target );

int otrReceive( SV* sv_otr_data, SV* sv_target );
int otrSend(SV* sv_msg, SV* sv_target);

void cHandleOtrMessage( char* accountname, char* username, char* msg );
void cHandleOtrInjectMessage( char* accountname, char* protocol, char* recipient, char* msg );
static void otrMessageDisconnect( ConnContext* ctx );


void cSetKeyFile( SV* sv_keyfile );
void cSetFingerprintFile( SV* sv_fprfile );
void cSetUserState( SV* sv_userstate );
void cSetID( SV* sv_id );
SV* cGetUserState();
SV* cGetID();

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
static char*	   cb_account_name		(void *opdata, const char* account, const char* protocol);
static void	   cb_account_name_free	(void *opdata, const char* account);		



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
		cb_account_name, /*account_name*/
		cb_account_name_free  /*account_name_free*/
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

	Inline_Stack_Vars;
	
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


int otrSend(SV* sv_msg, SV* sv_target)
{
	Inline_Stack_Vars;
	
	int err;
	int return_val = 0;
   
	printf( "hi\n" ); 
	
	SV* sv_user_state = cGetUserState();
	OtrlUserState userstate = SvIV( sv_user_state );

	printf( "otrSend userstate = %i\n", userstate );

	SV* sv_account_name = cGetID();
	char* account_name = SvPV_nolen( sv_account_name );
	
	printf( "otrSend using account name: %s\n", account_name);
	
	char* target = SvPV_nolen( sv_target );
	
	char *newmessage = NULL;
	char *msg;
	ConnContext * ctx;
	ctx = otrGetContext( sv_target );

	msg = strdup( SvPV_nolen(sv_msg) );
	
	printf( "otrl sending message: %s\n", msg );
	
	err = otrl_message_sending(userstate, &otrOps, NULL, account_name, PROTOCOL, target,
						  msg, NULL, &newmessage, NULL, NULL);
	
	printf( "msg = %s\nnewmsg = %s\n", msg, newmessage);
	
	if (err){
		free( msg );
		msg = NULL; /*something went wrong, don't send the plain-message! */
	}

	if (!err && newmessage) {
		free( msg );
		msg = strdup(newmessage);
		otrl_message_free(newmessage);
		if (cb_policy(NULL, ctx) & OTRL_POLICY_REQUIRE_ENCRYPTION ||
		    ctx->msgstate == OTRL_MSGSTATE_ENCRYPTED)
			return_val = 1;
	}
	
	sv_setpv( sv_msg, msg ); // set the correct return message
	
	if( msg )
		free( msg );

	return return_val;
}


/*
 * returns whether a otr_message was received
 * sets *otr_data to NULL, when it was an internal otr message
 */
int otrReceive( SV* sv_otr_data, SV* sv_target )
{
	int return_value = 0;
	int ignore_message;
	char *newmessage = NULL;
	OtrlTLV *tlvs = NULL;
	OtrlTLV *tlv = NULL;
	ConnContext * ctx;
	
	SV* perl_userstate = cGetUserState();
	OtrlUserState userstate = SvIV( perl_userstate );
	
	SV* perl_account = cGetID();
	char* account = SvPV_nolen( perl_account );
		
	char* target = SvPV_nolen( sv_target );

	char* otr_data = strdup( SvPV_nolen( sv_otr_data ) );
	
	int free_msg = 0;
	ignore_message = otrl_message_receiving(userstate, &otrOps, NULL, account,
									PROTOCOL, target, otr_data, &newmessage, &tlvs, NULL, NULL);

	ctx = otrGetContext( sv_target );

	tlv = otrl_tlv_find(tlvs, OTRL_TLV_DISCONNECTED);
	if (tlv) {
		/* Notify the user that the other side disconnected. */
		if (ctx) {
			cb_gone_insecure(NULL, ctx);
			otrDisconnect( sv_target );
		}
	}

	otr_handle_smp_tlvs(tlvs, ctx);

	if (tlvs != NULL)
		otrl_tlv_free(tlvs);

	if (ignore_message)
		otr_data = NULL;

	if (!ignore_message && newmessage) {
		free( otr_data );
		otr_data = strdup(newmessage);
		otrl_message_free(newmessage);
		if (ctx->msgstate == OTRL_MSGSTATE_ENCRYPTED)
			return_value = 1;
	}

   	sv_setpv( sv_otr_data, otr_data ); // set the correct return message
	
	free( otr_data );
	
	return return_value;
}


void otrEstablish( SV* target )
{
	otrStartstop( target, 1 );
}


void otrDisconnect( SV* target )
{
	otrStartstop( target, 0 );
}

void otrStartstop( SV* target, IV start )
{
	char* msg = NULL;
	ConnContext* ctx = otrGetContext( target );
	
	SV* perl_userstate = cGetUserState();
	OtrlUserState userstate = SvIV( perl_userstate );

	if( !userstate || !ctx )
		return;

	if( start && ctx->msgstate == OTRL_MSGSTATE_ENCRYPTED )
		otrMessageDisconnect( ctx );

	if( start ) {
		OtrlPolicy policy = cb_policy( NULL, ctx );
		// check policy here to make sure it iss set to encrypted
			
		printf( "Injecting OTR message\n" );
			
		msg = otrl_proto_default_query_msg( ctx->accountname, policy );
		cb_inject_message( NULL, ctx->accountname, ctx->protocol, ctx->username, msg );
	
		free( msg );
	}
	else
		otrMessageDisconnect( ctx );
}

static void otrMessageDisconnect( ConnContext* ctx )
{
	SV* perl_userstate = cGetUserState();
	OtrlUserState userstate = SvIV( perl_userstate );
	
	if( ctx->msgstate == OTRL_MSGSTATE_ENCRYPTED )
		cb_gone_insecure( NULL, ctx );

	otrl_message_disconnect( userstate, &otrOps, NULL, ctx->accountname, ctx->protocol, ctx->username );
}


// Looks up the context for the target in a global hash (stored on the Perl side
ConnContext* otrGetContext( SV* sv_target )
{
	int null = 0;
	ConnContext* ctx;
	char* target = SvPV_nolen( sv_target );
	
	SV* sv_account_name = cGetID();
	char* account_name = SvPV_nolen( sv_account_name );

	SV* perl_userstate = cGetUserState();
	OtrlUserState userstate = SvIV( perl_userstate );

	ctx = otrl_context_find( userstate, target, account_name, PROTOCOL, 1, &null, NULL, NULL );

	return ctx;
}


void otrCreatePrivKey( char* account_name, char* protocol )
{	  
	cb_create_privkey( NULL, account_name, protocol );
}


void cSetKeyFile( SV* sv_keyfile )
{
	Inline_Stack_Vars;
	
	printf( "Setting %s\n", SvPV_nolen( sv_keyfile ) );
	
	PUSHMARK(sp) ; 
	XPUSHs(sv_2mortal(newSVsv(sv_keyfile))); 
	PUTBACK; 
		
	perl_call_pv( "main::perlSetKeyFile", 0 );
	Inline_Stack_Void;
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
	Inline_Stack_Void;	
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
	Inline_Stack_Void;
}

SV* cGetUserState()
{
	
	Inline_Stack_Vars; // Set up stack shit

	perl_call_pv("main::perlGetUserState", G_SCALAR|G_NOARGS );
	SPAGAIN; // Adjust stack pointer for new addition
	SV* perl_user_state = POPs; // Pop it off
		
	printf( "Got userstate %i\n", SvIV( perl_user_state ) );
	return newSVsv( perl_user_state );
}

void cSetID( SV* sv_id )
{
	Inline_Stack_Vars;

	PUSHMARK( sp );
	XPUSHs(sv_2mortal(newSVsv(sv_id)));
	PUTBACK;
			
	perl_call_pv( "main::perlSetID", 0 );
	Inline_Stack_Void;
}

SV* cGetID()
{
	Inline_Stack_Vars;
	
	perl_call_pv("main::perlGetID", G_SCALAR|G_NOARGS );
	
	SV* perl_id = POPs;
	
	return newSVsv( perl_id );
}


void cOtrNotify( char* accountname, char* type, char* primary, char* secondary )
{

}

void cHandleOtrMessage( char* accountname, char* username, char* msg )
{
	Inline_Stack_Vars;

	printf( "cHandleOtrMessage: \n");
	printf( "username: %s\naccountname: %s\n%s\n", username, accountname, msg );
		
	PUSHMARK( sp );
	XPUSHs(sv_2mortal(newSVpv( accountname, strlen(accountname) + 1 )));
	XPUSHs(sv_2mortal(newSVpv( username, strlen( username ) + 1 )));
	XPUSHs(sv_2mortal(newSVpv( msg, strlen( msg ) + 1)));
	PUTBACK;
	
	perl_call_pv( "main::perlHandleOtrMessage", 0 );
	Inline_Stack_Void;
}

void cHandleOtrInjectMessage( char* accountname, char* protocol, char* recipient, char* msg )
{	
	Inline_Stack_Vars;
	
	printf( "cHandleOtrInjectMessage: \n");
	printf( "recipient: %s\naccountname: %s\n%sprotocol: %s\n%s\n", recipient, accountname, recipient, msg );
	
	PUSHMARK( sp );
	XPUSHs(sv_2mortal(newSVpv( accountname, strlen(accountname) + 1 )));
	XPUSHs(sv_2mortal(newSVpv( recipient, strlen( recipient ) + 1 )));
	XPUSHs(sv_2mortal(newSVpv( recipient, strlen( recipient ) + 1 )));
	XPUSHs(sv_2mortal(newSVpv( msg, strlen( msg ) + 1)));
	PUTBACK;
	
	perl_call_pv( "main::perlHandleOtrInjectMessage", 0 );
	Inline_Stack_Void;

}

void cHandleOtrNewFingerprint( char* accountname, char* readable )
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
	
	//puts( "cb_create_privkey" );
	
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

Inline_Stack_Vars;

	cHandleOtrInjectMessage( accountname, protocol, recipient, message );
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
	Inline_Stack_Vars;

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
	return strdup( protocol );
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

/* Return a newly allocated string containing a human-friendly
 * representation for the given account */
static char*	   cb_account_name		(void *opdata, const char* account, const char* protocol)
{
	SV* perl_account_name = cGetID();
	char* account_name = SvPV_nolen( perl_account_name );

	return strdup( account_name );
}


static void	   cb_account_name_free	(void *opdata, const char* account)
{
	// free perl stuff? I don't know how this is going to work
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




MODULE = otr_pm_51f9	PACKAGE = main	

PROTOTYPES: DISABLE


int
otrInit (client_id, userstate)
	SV *	client_id
	SV *	userstate

int
otrSend (sv_msg, sv_target)
	SV *	sv_msg
	SV *	sv_target

int
otrReceive (sv_otr_data, sv_target)
	SV *	sv_otr_data
	SV *	sv_target

void
otrEstablish (target)
	SV *	target
	PREINIT:
	I32* temp;
	PPCODE:
	temp = PL_markstack_ptr++;
	otrEstablish(target);
	if (PL_markstack_ptr != temp) {
          /* truly void, because dXSARGS not invoked */
	  PL_markstack_ptr = temp;
	  XSRETURN_EMPTY; /* return empty stack */
        }
        /* must have used dXSARGS; list context implied */
	return; /* assume stack size is correct */

void
otrDisconnect (target)
	SV *	target
	PREINIT:
	I32* temp;
	PPCODE:
	temp = PL_markstack_ptr++;
	otrDisconnect(target);
	if (PL_markstack_ptr != temp) {
          /* truly void, because dXSARGS not invoked */
	  PL_markstack_ptr = temp;
	  XSRETURN_EMPTY; /* return empty stack */
        }
        /* must have used dXSARGS; list context implied */
	return; /* assume stack size is correct */

void
otrStartstop (target, start)
	SV *	target
	IV	start
	PREINIT:
	I32* temp;
	PPCODE:
	temp = PL_markstack_ptr++;
	otrStartstop(target, start);
	if (PL_markstack_ptr != temp) {
          /* truly void, because dXSARGS not invoked */
	  PL_markstack_ptr = temp;
	  XSRETURN_EMPTY; /* return empty stack */
        }
        /* must have used dXSARGS; list context implied */
	return; /* assume stack size is correct */

void
otrCreatePrivKey (account_name, protocol)
	char *	account_name
	char *	protocol
	PREINIT:
	I32* temp;
	PPCODE:
	temp = PL_markstack_ptr++;
	otrCreatePrivKey(account_name, protocol);
	if (PL_markstack_ptr != temp) {
          /* truly void, because dXSARGS not invoked */
	  PL_markstack_ptr = temp;
	  XSRETURN_EMPTY; /* return empty stack */
        }
        /* must have used dXSARGS; list context implied */
	return; /* assume stack size is correct */

void
cSetKeyFile (sv_keyfile)
	SV *	sv_keyfile
	PREINIT:
	I32* temp;
	PPCODE:
	temp = PL_markstack_ptr++;
	cSetKeyFile(sv_keyfile);
	if (PL_markstack_ptr != temp) {
          /* truly void, because dXSARGS not invoked */
	  PL_markstack_ptr = temp;
	  XSRETURN_EMPTY; /* return empty stack */
        }
        /* must have used dXSARGS; list context implied */
	return; /* assume stack size is correct */

SV *
cGetKeyFile ()

void
cSetFingerprintFile (sv_fprfile)
	SV *	sv_fprfile
	PREINIT:
	I32* temp;
	PPCODE:
	temp = PL_markstack_ptr++;
	cSetFingerprintFile(sv_fprfile);
	if (PL_markstack_ptr != temp) {
          /* truly void, because dXSARGS not invoked */
	  PL_markstack_ptr = temp;
	  XSRETURN_EMPTY; /* return empty stack */
        }
        /* must have used dXSARGS; list context implied */
	return; /* assume stack size is correct */

SV *
cGetFingerprintFile ()

void
cSetUserState (sv_userstate)
	SV *	sv_userstate
	PREINIT:
	I32* temp;
	PPCODE:
	temp = PL_markstack_ptr++;
	cSetUserState(sv_userstate);
	if (PL_markstack_ptr != temp) {
          /* truly void, because dXSARGS not invoked */
	  PL_markstack_ptr = temp;
	  XSRETURN_EMPTY; /* return empty stack */
        }
        /* must have used dXSARGS; list context implied */
	return; /* assume stack size is correct */

SV *
cGetUserState ()

void
cSetID (sv_id)
	SV *	sv_id
	PREINIT:
	I32* temp;
	PPCODE:
	temp = PL_markstack_ptr++;
	cSetID(sv_id);
	if (PL_markstack_ptr != temp) {
          /* truly void, because dXSARGS not invoked */
	  PL_markstack_ptr = temp;
	  XSRETURN_EMPTY; /* return empty stack */
        }
        /* must have used dXSARGS; list context implied */
	return; /* assume stack size is correct */

SV *
cGetID ()

void
cOtrNotify (accountname, type, primary, secondary)
	char *	accountname
	char *	type
	char *	primary
	char *	secondary
	PREINIT:
	I32* temp;
	PPCODE:
	temp = PL_markstack_ptr++;
	cOtrNotify(accountname, type, primary, secondary);
	if (PL_markstack_ptr != temp) {
          /* truly void, because dXSARGS not invoked */
	  PL_markstack_ptr = temp;
	  XSRETURN_EMPTY; /* return empty stack */
        }
        /* must have used dXSARGS; list context implied */
	return; /* assume stack size is correct */

void
cHandleOtrMessage (accountname, username, msg)
	char *	accountname
	char *	username
	char *	msg
	PREINIT:
	I32* temp;
	PPCODE:
	temp = PL_markstack_ptr++;
	cHandleOtrMessage(accountname, username, msg);
	if (PL_markstack_ptr != temp) {
          /* truly void, because dXSARGS not invoked */
	  PL_markstack_ptr = temp;
	  XSRETURN_EMPTY; /* return empty stack */
        }
        /* must have used dXSARGS; list context implied */
	return; /* assume stack size is correct */

void
cHandleOtrInjectMessage (accountname, protocol, recipient, msg)
	char *	accountname
	char *	protocol
	char *	recipient
	char *	msg
	PREINIT:
	I32* temp;
	PPCODE:
	temp = PL_markstack_ptr++;
	cHandleOtrInjectMessage(accountname, protocol, recipient, msg);
	if (PL_markstack_ptr != temp) {
          /* truly void, because dXSARGS not invoked */
	  PL_markstack_ptr = temp;
	  XSRETURN_EMPTY; /* return empty stack */
        }
        /* must have used dXSARGS; list context implied */
	return; /* assume stack size is correct */

void
cHandleOtrNewFingerprint (accountname, readable)
	char *	accountname
	char *	readable
	PREINIT:
	I32* temp;
	PPCODE:
	temp = PL_markstack_ptr++;
	cHandleOtrNewFingerprint(accountname, readable);
	if (PL_markstack_ptr != temp) {
          /* truly void, because dXSARGS not invoked */
	  PL_markstack_ptr = temp;
	  XSRETURN_EMPTY; /* return empty stack */
        }
        /* must have used dXSARGS; list context implied */
	return; /* assume stack size is correct */

char *
expandFilename (fname)
	const char *	fname

