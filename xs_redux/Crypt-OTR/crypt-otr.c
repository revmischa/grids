

/* An attempt to use perl's threadsafe storage for static data failed */ 

/*
OtrlUserState crypt_otr_get_userstate() { return MY_CXT.userstate; }
char* crypt_otr_get_keyfile() { return MY_CXT.keyfile; }
char* crypt_otr_get_fprfile() { return MY_CXT.fprfile; }
char* crypt_otr_get_accountname() { return MY_CXT.accountname; }
char* crypt_otr_get_protocol() { return MY_CXT.protocol; }
int crypt_otr_get_max_message_size() { return MY_CXT.max_message_size; }

char* crypt_otr_get_inject_cb() { return MY_CXT.inject_cb; }
char* crypt_otr_get_display_cb() { return MY_CXT.display_cb; }
char* crypt_otr_get_error_cb() { return MY_CXT.error_cb; }
char* crypt_otr_get_warning_cb() { return MY_CXT.warning_cb; }
char* crypt_otr_get_info_cb() { return MY_CXT.info_cb; }


void crypt_otr_set_userstate( OtrlUserState userstate ) { MY_CXT.userstate = userstate; }
void crypt_otr_set_keyfile( char* keyfile ) { MY_CXT.keyfile = keyfile; }
void crypt_otr_set_fprfile( char* fprfile ) { MY_CXT.fprfile = fprfile; }
void crypt_otr_set_accountname( char* accountname ) { MY_CXT.accountname = accountname; }
void crypt_otr_set_protocol( char* protocol ) { MY_CXT.protocol = protocol; }
void crypt_otr_set_max_message_size ( int max_size ) { MY_CXT.max_message_size = max_size; }
*/


OtrlUserState crypt_otr_userstate;
char* crypt_otr_keyfile;
char* crypt_otr_fprfile;
char* crypt_otr_accountname;
char* crypt_otr_protocol;
unsigned int crypt_otr_max_size;

char* crypt_otr_inject_cb;
char* crypt_otr_display_cb;
char* crypt_otr_connected_cb;
char* crypt_otr_disconnected_cb;
char* crypt_otr_error_cb;
char* crypt_otr_warning_cb;
char* crypt_otr_info_cb;
char* crypt_otr_new_fpr_cb;

OtrlUserState crypt_otr_get_userstate() { return crypt_otr_userstate; }
char* crypt_otr_get_keyfile() { return crypt_otr_keyfile; }
char* crypt_otr_get_fprfile() { return crypt_otr_fprfile; }
char* crypt_otr_get_accountname() { return crypt_otr_accountname; }
char* crypt_otr_get_protocol() { return crypt_otr_protocol; }
unsigned int crypt_otr_get_max_message_size() { return crypt_otr_max_size; }

char* crypt_otr_get_inject_cb() { return  crypt_otr_inject_cb; }
char* crypt_otr_get_display_cb() { return crypt_otr_display_cb; }
char* crypt_otr_get_error_cb() { return crypt_otr_error_cb; }
char* crypt_otr_get_warning_cb() { return crypt_otr_warning_cb; }
char* crypt_otr_get_info_cb() { return crypt_otr_info_cb; }

void crypt_otr_set_userstate( OtrlUserState in_userstate ) { crypt_otr_userstate = in_userstate; }
void crypt_otr_set_keyfile( char* in_keyfile ) { crypt_otr_keyfile = in_keyfile; }
void crypt_otr_set_fprfile( char* in_fprfile ) { crypt_otr_fprfile = in_fprfile; }
void crypt_otr_set_accountname( char* in_accountname ) { crypt_otr_accountname = in_accountname; }
void crypt_otr_set_protocol( char* in_protocol ) { crypt_otr_protocol = in_protocol; }
void crypt_otr_set_max_message_size ( int in_max_size ) { crypt_otr_max_size = in_max_size; }

void crypt_otr_set_inject_cb( char* in_inject_cb ){ crypt_otr_inject_cb = in_inject_cb; }
void crypt_otr_set_display_cb( char* in_display_cb ){ crypt_otr_display_cb = in_display_cb; }
void crypt_otr_set_connected_cb( char* in_connected_cb ){ crypt_otr_connected_cb = in_connected_cb; }
void crypt_otr_set_disconnected_cb( char* in_disconnected_cb ){ crypt_otr_disconnected_cb = in_disconnected_cb; }
void crypt_otr_set_error_cb( char* in_error_cb ) { crypt_otr_error_cb = in_error_cb; }
void crypt_otr_set_warning_cb( char* in_warning_cb ) { crypt_otr_warning_cb = in_warning_cb; }
void crypt_otr_set_info_cb( char* in_info_cb ) { crypt_otr_info_cb = in_info_cb; }
void crypt_otr_set_new_fpr_cb( char* in_fpr_cb ) { crypt_otr_new_fpr_cb = in_fpr_cb; }



///////////////////////////////////
// CALLBACKS
//////////////////////////////////

/* Return the OTR policy for the given context. */
static OtrlPolicy policy_cb(void *opdata, ConnContext *context)
{
	// TODO:
	// Figure out how to keep track of accountname / username policy pairs
	/*
    	OtrlPolicy policy = OTRL_POLICY_DEFAULT;
    
    if (!context) return policy;

    return crypt_otr_get_policy(account, context->username);
	*/
	
	return OTRL_POLICY_ALWAYS & ~OTRL_POLICY_ALLOW_V1; // I believe this means you will always do OTR
}

static const char *protocol_name_cb(void *opdata, const char *protocol)
{
	return strdup(protocol);
}

static void protocol_name_free_cb(void *opdata, const char *protocol_name)
{
	free( protocol_name );
}

static void create_privkey_cb(void *opdata, const char *accountname,
	const char *protocol)
{
    crypt_otr_create_privkey(accountname, protocol);
}

/* Report whether you think the given user is online.  Return 1 if
 * you think he is, 0 if you think he isn't, -1 if you're not sure.
 * If you return 1, messages such as heartbeats or other
 * notifications may be sent to the user, which could result in "not
 * logged in" errors if you're wrong. */
static int is_logged_in_cb(void *opdata, const char *accountname,
	const char *protocol, const char *recipient)
{
	return 1; // Maybe -1, though for now we will always assume there is a proper connection
}

/* Send the given IM to the given recipient from the given
 * accountname/protocol. */
static void inject_message_cb(void *opdata, const char *accountname,
	const char *protocol, const char *recipient, const char *message)
{    
	puts( "Injecting message" );
	crypt_otr_inject_message(accountname, protocol, recipient, message);
}


/* Display a notification message for a particular accountname /
 * protocol / username conversation. */
static void notify_cb(void *opdata, OtrlNotifyLevel level,
				  const char *accountname, const char *protocol, const char *username,
				  const char *title, const char *primary, const char *secondary)
{
	crypt_otr_notify( level, accountname, protocol, username, title, primary, secondary );
}

/* Return / display a decrypted message */

/* Display an OTR control message for a particular accountname /
 * protocol / username conversation.  Return 0 if you are able to
 * successfully display it.  If you return non-0 (or if this
 * function is NULL), the control message will be displayed inline,
 * as a received message, or else by using the above notify()
 * callback. */
static int display_otr_message_cb(void *opdata, const char *accountname,
						    const char *protocol, const char *username, const char *msg)
{
	return crypt_otr_display_otr_message(accountname, protocol, username, msg);
}

/* When the list of ConnContexts changes (including a change in
 * state), this is called so the UI can be updated. */
static void update_context_list_cb(void *opdata)
{
	// There is no UI so there is nothing to do here
}

static void confirm_fingerprint_cb(void *opdata, OtrlUserState us,
	const char *accountname, const char *protocol, const char *username,
	unsigned char fingerprint[20])
{
	char readable[45];

	otrl_privkey_hash_to_human(readable, fingerprint);

	crypt_otr_new_fingerprint( accountname, protocol, username, readable );
}

static void write_fingerprints_cb(void *opdata)
{
	otrl_privkey_write_fingerprints( crypt_otr_get_userstate(), crypt_otr_get_fprfile() );
}

static void gone_secure_cb(void *opdata, ConnContext *context)
{
    crypt_otr_handle_connected(context);
}

static void gone_insecure_cb(void *opdata, ConnContext *context)
{
    crypt_otr_handle_disconnected(context->username);
}

static void still_secure_cb(void *opdata, ConnContext *context, int is_reply)
{
    if (is_reply == 0) {
	crypt_otr_handle_stillconnected(context->username);
    }
}

static void log_message_cb(void *opdata, const char *message)
{
    printf("otr: %s", message);
}

static int max_message_size_cb(void *opdata, ConnContext *context)
{
	return crypt_otr_get_max_message_size();
}

/* Return a newly allocated string containing a human-friendly
 * representation for the given account */
static const char* account_name_cb( void *opdata, const char *account, const char *protocol )
{
	strdup( crypt_otr_get_accountname() );
}

/* Deallocate a string returned by account_name */
static void account_name_free_cb(void *opdata, const char *account_name)
{
	free( account_name );
}

static OtrlMessageAppOps otr_ops = {
    policy_cb,
    create_privkey_cb,
    is_logged_in_cb,
    inject_message_cb,
    notify_cb,
    display_otr_message_cb,
    update_context_list_cb,
    protocol_name_cb,
    protocol_name_free_cb,
    confirm_fingerprint_cb,
    write_fingerprints_cb,
    gone_secure_cb,
    gone_insecure_cb,
    still_secure_cb,
    log_message_cb,
    max_message_size_cb,
    account_name_cb,                   
    account_name_free_cb  
};




////////////////////////////////////////////////
// CRYPT_OTR FUNCTIONS
///////////////////////////////////////////////


static void
crypt_otr_inject_message( const char* account, const char* protocol, const char* recipient, const char* message )
{
	dSP;
	
	ENTER;
	SAVETMPS;

	PUSHMARK(SP);
	XPUSHs( sv_2mortal( newSVpv( account, 0 ))); // The 0 may have to be strlen() ...
	XPUSHs( sv_2mortal( newSVpv( protocol, 0 )));
	XPUSHs( sv_2mortal( newSVpv( recipient, 0 )));
	XPUSHs( sv_2mortal( newSVpv( message, 0 )));
	PUTBACK;

	call_pv( crypt_otr_get_inject_cb(), G_DISCARD );
	//call_pv( "main::perl_inject_message", G_DISCARD );
	
	FREETMPS;
	LEAVE;
}

static int 
crypt_otr_display_otr_message( const char* accountname, const char* protocol, const char* username, const char* message )
{
	dSP;
	int num_items_on_stack;
	
	ENTER;
	SAVETMPS;

	PUSHMARK(SP);
	XPUSHs( sv_2mortal( newSVpv( accountname, 0 ))); // The 0 may have to be strlen() ...
	XPUSHs( sv_2mortal( newSVpv( protocol, 0 )));
	XPUSHs( sv_2mortal( newSVpv( username, 0 )));
	XPUSHs( sv_2mortal( newSVpv( message, 0 )));
	PUTBACK;

	num_items_on_stack = call_pv( crypt_otr_get_display_cb(), G_DISCARD );
	
	FREETMPS;
	LEAVE;
	
	return num_items_on_stack == 0 ? 1 : 0;
}


void crypt_otr_notify( OtrlNotifyLevel level, const char* accountname, const char* protocol, const char* username, const char* title, const char* primary, const char* secondary )
{
	dSP;
	
	ENTER;
	SAVETMPS;

	PUSHMARK(SP);
	XPUSHs( sv_2mortal( newSVpv( accountname, 0 ))); 
	XPUSHs( sv_2mortal( newSVpv( protocol, 0 )));
	XPUSHs( sv_2mortal( newSVpv( username, 0 )));
	XPUSHs( sv_2mortal( newSVpv( title, 0 )));
	XPUSHs( sv_2mortal( newSVpv( primary, 0 )));
	XPUSHs( sv_2mortal( newSVpv( secondary, 0 )));
	PUTBACK;

	switch (level) {
	case OTRL_NOTIFY_ERROR:
		call_pv( crypt_otr_get_error_cb(), G_DISCARD );
		break;
	case OTRL_NOTIFY_WARNING:
		call_pv( crypt_otr_get_warning_cb(), G_DISCARD );
		break;
	case OTRL_NOTIFY_INFO:
		call_pv( crypt_otr_get_notify_cb(), G_DISCARD );
		break;
	}
	
	FREETMPS;
	LEAVE;
}


void crypt_otr_handle_connected(ConnContext* context)
{	
	char* username = context->username;
	TrustLevel level;

	level = otrg_plugin_context_to_trust(context);

	switch(level) {
	case TRUST_PRIVATE:
		crypt_otr_handle_trusted_connection( username );
		break;

	case TRUST_UNVERIFIED:
		crypt_otr_handle_unverified_connection( username );
		break;

	default:
		/* This last case should never happen, since we know
		 * we're in ENCRYPTED. */
		printf( "ERROR -- Unencrypted conversation started\n" );
		break;
	}
}

void crypt_otr_handle_trusted_connection( char* username )
{
	/* Send the username, basically saying that a trusted conversation has been started with username */
} 

void crypt_otr_handle_unverified_connection( char* username )
{
	/* Send the username, basically saying that an unverified conversation has been started with username */
}

void crypt_otr_handle_disconnection( char* username )
{
	/* Send the username, saying that a conversation has ended  with username */
}

void crypt_otr_handle_stillconnected( char* username )
{
	/* Send the username, saying that a conversation has ended  with username */
}

/* Abort the SMP protocol.  Used when malformed or unexpected messages
 * are received. */
void crypt_otr_abort_smp( ConnContext* context )
{
	otrl_message_abort_smp( crypt_otr_get_userstate(), &otr_ops, NULL, context);
}

void crypt_otr_completed_smp( ConnContext* context )
{

}

void crypt_otr_ask_socialist_millionaires_q( ConnContext* context, char* question )
{

}

void crypt_otr_ask_socialist_millionaires( ConnContext* context )
{


}

int crypt_otr_context_to_trust(ConnContext *context)
{
    TrustLevel level = TRUST_NOT_PRIVATE;

    if (context && context->msgstate == OTRL_MSGSTATE_ENCRYPTED) {
	if (context->active_fingerprint->trust &&
		context->active_fingerprint->trust[0] != '\0') {
	    level = TRUST_PRIVATE;
	} else {
	    level = TRUST_UNVERIFIED;
	}
    } else if (context && context->msgstate == OTRL_MSGSTATE_FINISHED) {
	level = TRUST_FINISHED;
    }

    return level;
}

/* Generate a private key for the given accountname/protocol */
void crypt_otr_create_privkey(const char *accountname,
						const char *protocol)
{
    	int key_error;
		
	OtrlUserState userstate = crypt_otr_get_userstate(); // extract the pointer
	printf( "Userstate extracted %i\n", userstate );
	
	char* keyfile = crypt_otr_get_keyfile();	
	printf( "Keyfile extracted: %s\n", keyfile);

	printf( "Generating new OTR key for %s.\nThis may take a while... (like several minutes srsly)\n", accountname);

	key_error = otrl_privkey_generate( userstate, keyfile, accountname, protocol );

	if( key_error ) {
		printf("***********************************\n");
		printf("OTR key generation failed!  Please make the following directory: %s\n", keyfile);		
		printf("***********************************\n");
	}
	else {
		printf("OTR key generated.\n");
	}   
}


 
void crypt_otr_startstop( char* username, int start )
{	
	char* msg = NULL;
	ConnContext* ctx = crypt_otr_get_context( username );
	
	OtrlUserState userstate = crypt_otr_get_userstate();

	printf( "crypt_otr_startstop userstate: %i\ncontext: %i\nusername: %s\n", userstate, ctx, username );
	
	if( !userstate || !ctx )
		return;
	
	/* Let the user know that the conversation has been disconnected */
	if( start && ctx->msgstate == OTRL_MSGSTATE_ENCRYPTED )
		crypt_otr_message_disconnect( ctx );

	if( start ) {
		OtrlPolicy policy = policy_cb( NULL, ctx );
		// check policy here to make sure it iss set to encrypted
			
		printf( "Injecting OTR message\n" );
			
		msg = otrl_proto_default_query_msg( ctx->accountname, policy );
		inject_message_cb( NULL, ctx->accountname, ctx->protocol, ctx->username, msg );
	
		free( msg );
	}
	else
		crypt_otr_message_disconnect( ctx );
}

static void crypt_otr_message_disconnect( ConnContext* ctx )
{	
	OtrlUserState userstate = crypt_otr_get_userstate();
	
	if( ctx->msgstate == OTRL_MSGSTATE_ENCRYPTED )
		gone_insecure_cb( NULL, ctx );

	otrl_message_disconnect( userstate, &otr_ops, NULL, ctx->accountname, ctx->protocol, ctx->username );
}


/* Looks up the context for the target in a global hash (stored on the Perl side */
ConnContext* crypt_otr_get_context( char* username )
{
	int null = 0;
	ConnContext* ctx;
		
	char* accountname = crypt_otr_get_accountname();	
	OtrlUserState userstate = crypt_otr_get_userstate();
	char* protocol = crypt_otr_get_protocol();	

	printf( "crypt_otr_get_context accountname: %s\nuserstate: %i\nprotocol: %s\n", accountname, userstate, protocol );
	
	ctx = otrl_context_find( userstate, username, accountname, protocol, 1, &null, NULL, NULL );

	return ctx;
}






/////////////////////////////////////////
//  CRYPT-OTR PERL FUNCTIONS
////////////////////////////////////////


/* crypt_otr_init
 * Call OTRL_INIT
 * Create a private key if there is not one already
 * Store values of accountname (your name), the protocol
 */

//int crypt_otr_init( SV* sv_accountname, SV* sv_protocolname, SV* sv_max_message_size )
int crypt_otr_init(  )
{
	char* root;
	char* temp_keyfile;
	char* temp_fingerprintfile;
	
	char* accountname = crypt_otr_get_accountname();
	char* protocol = crypt_otr_get_protocol();
	int max_msg = crypt_otr_get_max_message_size();

	OtrlUserState userstate = crypt_otr_get_userstate();
		
	if( userstate ){
		return 0;
	}

	OTRL_INIT;

	userstate = otrl_userstate_create();	
	crypt_otr_set_userstate( userstate );
	printf( "userstate ptr = %i\n", userstate );
				
	root = expand_filename( "~/.grids/otr/" );
	
	temp_keyfile = malloc( (strlen(root) + strlen(accountname) + strlen(".key") + 1)*sizeof(char) ); // +1 for the \0
	temp_fingerprintfile = malloc( (strlen(root) + strlen( accountname) + strlen(".fpr") + 1)*sizeof(char) );
	sprintf( temp_keyfile, "%s%s.key", root, accountname);
	sprintf( temp_fingerprintfile, "%s%s.fpr", root, accountname);
				
	crypt_otr_set_keyfile( temp_keyfile );
	crypt_otr_set_fprfile( temp_fingerprintfile );

	if( otrl_privkey_read( userstate, temp_keyfile ) ) {
		printf( "Could not read OTR key from %s\n", temp_keyfile);
		crypt_otr_create_privkey( accountname, protocol );
	}
	else {
		printf( "Loaded private key file from %s\n", temp_keyfile );
	}

	free( temp_keyfile );
	free( temp_fingerprintfile );
	
	return 0;
}


void crypt_otr_establish( char* username )
{
	//puts( "crypt_otr_establish" );
	crypt_otr_startstop( username, 1 );
}


void crypt_otr_disconnect( char* username )
{
	crypt_otr_startstop( username, 0 );
}


void crypt_otr_process_sending(char* who, char* message )
{
	char* newmessage = NULL;
	OtrlUserState userstate = crypt_otr_get_userstate();
	const char* accountname = crypt_otr_get_accountname();
	const char* protocol = crypt_otr_get_protocol();
	char* username = who;
	int err;
	
	if( !who || !message )
		return;
	
	err = otrl_message_sending( userstate, &otr_ops, NULL, 
						   accountname, protocol, username, 
						   message, NULL, &newmessage, NULL, NULL);

	if( err && newmessage == NULL ) {
		/* Be *sure* not to send out plaintext */
		char* ourm = strdup( "" );
		free( message ); // This may cause bugs, I don't know how perl allocates memory, though it's probably with strdup
		message = ourm;
	} else if ( newmessage ) {
		/* Fragment the message if necessary, and send all but the last
		 * fragment over the network.  The client will send the last
		 * fragment for us. */
		ConnContext* context = otrl_context_find( userstate, username, accountname, 
										  protocol, 0, NULL, NULL, NULL );
		free( message );
		message = NULL;
		err = otrl_message_fragment_and_send(&otr_ops, NULL, context,
									  newmessage, OTRL_FRAGMENT_SEND_ALL_BUT_LAST, message);
		otrl_message_free(newmessage);
	}
}


/*
 * returns whether a otr_message was received
 * sets *message to NULL, when it was an internal otr message
 */
static int crypt_otr_process_receiving( char* who, char* message )
{
	char* newmessage = NULL;
	OtrlTLV* tlvs = NULL;
	OtrlTLV* tlv = NULL;
	OtrlUserState userstate;
	char* username;
	int res;
	const char* accountname;
	const char* protocol;
	ConnContext* context;
	NextExpectedSMP nextMsg;

	if( !who || !message )
		return 0;
	
	userstate = crypt_otr_get_userstate();
	username = who;
	accountname = crypt_otr_get_accountname();
	protocol = crypt_otr_get_protocol();

	res = otrl_message_receiving( userstate, &otr_ops, NULL, 
							accountname, protocol, username, message,
							&newmessage, &tlvs, NULL, NULL );
	
	if( newmessage ) {
		char* ourm = malloc( strlen( newmessage ) + 1 );
		if( ourm ) {
			strcpy( ourm, newmessage );
		}
		otrl_message_free( newmessage );
		free( message ); // this may cause problems, if perl doesn't use malloc, or tries cleaning up the pointer itself
		message = ourm;
	}

	tlv = otrl_tlv_find( tlvs, OTRL_TLV_DISCONNECTED );
	if( tlv ) {
		/* Notify the user that the other side disconnected */
		crypt_otr_handle_disconnection( username );
	}

	/* Keep track of our current progress in the Socialist Millionaires'
	 * Protocol. */
	context = otrl_context_find( userstate, username, accountname, protocol, 0, NULL, NULL, NULL );
	if( context ) {
		nextMsg = context->smstate->nextExpected;

		if( context->smstate->sm_prog_state == OTRL_SMP_PROG_CHEATED ) {
			crypt_otr_abort_smp( context );
			context->smstate->nextExpected = OTRL_SMP_EXPECT1;
			context->smstate->sm_prog_state = OTRL_SMP_PROG_OK;
		} else {
			tlv = otrl_tlv_find(tlvs, OTRL_TLV_SMP1Q);
			if (tlv) {
				if (nextMsg != OTRL_SMP_EXPECT1)
					crypt_otr_abort_smp(context);
				else {
					char *question = (char *)tlv->data;
					char *eoq = memchr(question, '\0', tlv->len);
					if (eoq) {
						crypt_otr_ask_socialist_millionaires_q(context, question);
					}
				}
			}
			tlv = otrl_tlv_find(tlvs, OTRL_TLV_SMP1);
			if (tlv) {
				if (nextMsg != OTRL_SMP_EXPECT1)
					crypt_otr_abort_smp(context);
				else {
					crypt_otr_ask_socialist_millionaires(context);
				}
			}
			tlv = otrl_tlv_find(tlvs, OTRL_TLV_SMP2);
			if (tlv) {
				if (nextMsg != OTRL_SMP_EXPECT2)
					crypt_otr_abort_smp(context);
				else {
					context->smstate->nextExpected = OTRL_SMP_EXPECT4;
				}
			}
			tlv = otrl_tlv_find(tlvs, OTRL_TLV_SMP3);
			if (tlv) {
				if (nextMsg != OTRL_SMP_EXPECT3)
					crypt_otr_abort_smp(context);
				else {
					crypt_otr_completed_smp( context );
					context->smstate->nextExpected = OTRL_SMP_EXPECT1;
				}
			}
			tlv = otrl_tlv_find(tlvs, OTRL_TLV_SMP4);
			if (tlv) {
				if (nextMsg != OTRL_SMP_EXPECT4)
					crypt_otr_abort_smp(context);
				else {
					crypt_otr_completed_smp( context );
					context->smstate->nextExpected = OTRL_SMP_EXPECT1;
				}
			}
			tlv = otrl_tlv_find(tlvs, OTRL_TLV_SMP_ABORT);
			if (tlv) {				
				context->smstate->nextExpected = OTRL_SMP_EXPECT1;
			}
		}
	}

	otrl_tlv_free(tlvs);

	/* If we're supposed to ignore this incoming message (because it's a
	 * protocol message), set it to NULL, so that other plugins that
	 * catch receiving-im-msg don't return 0, and cause it to be
	 * displayed anyway. */
	if (res) {
		free(message);
		message = NULL;
	}
	return res;
}





////////////////////////////////////
// Utilities
///////////////////////////////////


//  expandFilename(filename)
// Expand "~/" with the $HOME env. variable in a file name.
// The caller must free the string after use.
char *expand_filename(const char *fname)
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

