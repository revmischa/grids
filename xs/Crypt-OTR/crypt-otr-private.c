


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
	XPUSHs( sv_2mortal( newSVpv( account, 0 )));
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
	XPUSHs( sv_2mortal( newSVpv( accountname, 0 ))); // The 0 causes perl to calculate the Strlen ...
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

	//printf( "***************\nCONNECTED\n***************\n");

	level = crypt_otr_context_to_trust(context);

	//printf( "Got level\n" );

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

void crypt_otr_callback_one_string( char* callback_sub, char* username )
{
	dSP;
	
	ENTER;
	SAVETMPS;

	PUSHMARK(SP);
	XPUSHs( sv_2mortal( newSVpv( username, 0 ))); 
	PUTBACK;

	call_pv( callback_sub, G_DISCARD );
	
	FREETMPS;
	LEAVE;

}

/* Send the username, basically saying that a trusted conversation has been started with username */
void crypt_otr_handle_trusted_connection( char* username )
{
	crypt_otr_callback_one_string( crypt_otr_get_connected_cb(), username );
} 

/* Send the username, basically saying that an unverified conversation has been started with username */
void crypt_otr_handle_unverified_connection( char* username )
{
	crypt_otr_callback_one_string( crypt_otr_get_unverified_cb(), username );
}

/* Send the username, saying that a conversation has ended  with username */
void crypt_otr_handle_disconnection( char* username )
{
	crypt_otr_callback_one_string( crypt_otr_get_disconnected_cb(), username );
}

/* Send the username, saying that is still connected  with username */
void crypt_otr_handle_stillconnected( char* username )
{
	crypt_otr_callback_one_string( crypt_otr_get_stillconnected_cb(), username );
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

void crypt_otr_new_fingerprint( const char* accountname, const char* protocol, const char* username, unsigned char fingerprint[20] )
{






} 
