
#define context otr_context
#include <libotr/context.h>
#undef context

#include <libotr/proto.h>
#include <libotr/message.h>
#include <libotr/privkey.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *expand_filename(const char *fname);

void 		crypt_otr_handle_connected(ConnContext* context);
void 		crypt_otr_handle_trusted_connection( char* username );
void 		crypt_otr_handle_unverified_connection( char* username );
void 		crypt_otr_handle_disconnection( char* username );
void 		crypt_otr_handle_stillconnected( char* username );

static int 	crypt_otr_display_otr_message( const char* accountname, const char* protocol, const char* username, const char* message );
static void 	crypt_otr_inject_message( const char* account, const char* protocol, const char* recipient, const char* message );

void crypt_otr_notify( OtrlNotifyLevel level, const char* accountname, const char* protocol, const char* username, const char* title, const char* primary, const char* secondary );

static void 	crypt_otr_message_disconnect( ConnContext* ctx );
ConnContext* 	crypt_otr_get_context( char* username );
void 		crypt_otr_create_privkey(const char *accountname, const char *protocol);

void process_sending_im( char* who, char* message );

/* Callbacks */
static OtrlPolicy 	policy_cb(void *opdata, ConnContext *context);
static const char *	protocol_name_cb(void *opdata, const char *protocol);
static void 		protocol_name_free_cb(void *opdata, const char *protocol_name);
static void 		create_privkey_cb(void *opdata, const char *accountname,
							   const char *protocol);
static int 		is_logged_in_cb(void *opdata, const char *accountname,
							 const char *protocol, const char *recipient);
static void 		inject_message_cb(void *opdata, const char *accountname,
							   const char *protocol, const char *recipient, const char *message);
static void 		notify_cb(void *opdata, OtrlNotifyLevel level,
						const char *accountname, const char *protocol, const char *username,
						const char *title, const char *primary, const char *secondary);
static int 		display_otr_message_cb(void *opdata, const char *accountname, const char *protocol, const char *username, const char *msg);
static void 		update_context_list_cb(void *opdata);
static void 		confirm_fingerprint_cb(void *opdata, OtrlUserState us, const char *accountname, const char *protocol, const char *username, unsigned char fingerprint[20]);
static void 		write_fingerprints_cb(void *opdata);
static void 		gone_secure_cb(void *opdata, ConnContext *context);
static void 		gone_insecure_cb(void *opdata, ConnContext *context);
static void 		still_secure_cb(void *opdata, ConnContext *context, int is_reply);
static void 		log_message_cb(void *opdata, const char *message);
static int 		max_message_size_cb(void *opdata, ConnContext *context);
static const char* 	account_name_cb( void *opdata, const char *account, const char *protocol );
static void 		account_name_free_cb(void *opdata, const char *account_name);


typedef enum {
    TRUST_NOT_PRIVATE,
    TRUST_UNVERIFIED,
    TRUST_PRIVATE,
    TRUST_FINISHED
} TrustLevel;

int crypt_otr_context_to_trust(ConnContext *context);

#include "crypt-otr-utils.c"
#include "crypt-otr-members.c"
#include "crypt-otr-callbacks.c"
#include "crypt-otr-private.c"
#include "crypt-otr-perl.c"
