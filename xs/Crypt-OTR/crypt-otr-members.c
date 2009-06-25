/* An attempt to use perl's threadsafe storage for static data failed */ 
/* This is how you could implement threading */

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
char* crypt_otr_root;
char* crypt_otr_keyfile;
char* crypt_otr_fprfile;
char* crypt_otr_accountname;
char* crypt_otr_protocol;
unsigned int crypt_otr_max_size;

char* crypt_otr_inject_cb;
char* crypt_otr_display_cb;
char* crypt_otr_connected_cb;
char* crypt_otr_unverified_cb;
char* crypt_otr_disconnected_cb;
char* crypt_otr_stillconnected_cb;
char* crypt_otr_error_cb;
char* crypt_otr_warning_cb;
char* crypt_otr_info_cb;
char* crypt_otr_new_fpr_cb;

OtrlUserState crypt_otr_get_userstate() { return crypt_otr_userstate; }
char* crypt_otr_get_keyfile() { return crypt_otr_keyfile; }
char* crypt_otr_get_fprfile() { return crypt_otr_fprfile; }
char* crypt_otr_get_root() { return crypt_otr_root; }
char* crypt_otr_get_accountname() { return crypt_otr_accountname; }
char* crypt_otr_get_protocol() { return crypt_otr_protocol; }
unsigned int crypt_otr_get_max_message_size() { return crypt_otr_max_size; }

char* crypt_otr_get_inject_cb() { return  crypt_otr_inject_cb; }
char* crypt_otr_get_display_cb() { return crypt_otr_display_cb; }
char* crypt_otr_get_connected_cb() { return crypt_otr_connected_cb; }
char* crypt_otr_get_unverified_cb() { return crypt_otr_unverified_cb; }
char* crypt_otr_get_disconnected_cb() { return crypt_otr_disconnected_cb; }
char* crypt_otr_get_stillconnected_cb() { return crypt_otr_stillconnected_cb; }
char* crypt_otr_get_error_cb() { return crypt_otr_error_cb; }
char* crypt_otr_get_warning_cb() { return crypt_otr_warning_cb; }
char* crypt_otr_get_info_cb() { return crypt_otr_info_cb; }
char* crypt_otr_get_new_fpr_cb() { return crypt_otr_new_fpr_cb; }


void crypt_otr_set_userstate( OtrlUserState in_userstate ) { crypt_otr_userstate = in_userstate; }
void crypt_otr_set_keyfile( char* in_keyfile ) { crypt_otr_keyfile = in_keyfile; }
void crypt_otr_set_fprfile( char* in_fprfile ) { crypt_otr_fprfile = in_fprfile; }
void crypt_otr_set_root( char* in_root ) { crypt_otr_root = in_root; }
void crypt_otr_set_accountname( char* in_accountname ) { crypt_otr_accountname = in_accountname; }
void crypt_otr_set_protocol( char* in_protocol ) { crypt_otr_protocol = in_protocol; }
void crypt_otr_set_max_message_size ( int in_max_size ) { crypt_otr_max_size = in_max_size; }

void crypt_otr_set_inject_cb( char* in_inject_cb ){ crypt_otr_inject_cb = in_inject_cb; }
void crypt_otr_set_display_cb( char* in_display_cb ){ crypt_otr_display_cb = in_display_cb; }
void crypt_otr_set_connected_cb( char* in_connected_cb ){ crypt_otr_connected_cb = in_connected_cb; }
void crypt_otr_set_unverified_cb( char* in_unver_cb ) { crypt_otr_unverified_cb = in_unver_cb; }
void crypt_otr_set_disconnected_cb( char* in_disconnected_cb ){ crypt_otr_disconnected_cb = in_disconnected_cb; }
void crypt_otr_set_stillconnected_cb( char* in_still_cb ) { crypt_otr_stillconnected_cb = in_still_cb; }
void crypt_otr_set_error_cb( char* in_error_cb ) { crypt_otr_error_cb = in_error_cb; }
void crypt_otr_set_warning_cb( char* in_warning_cb ) { crypt_otr_warning_cb = in_warning_cb; }
void crypt_otr_set_info_cb( char* in_info_cb ) { crypt_otr_info_cb = in_info_cb; }
void crypt_otr_set_new_fpr_cb( char* in_fpr_cb ) { crypt_otr_new_fpr_cb = in_fpr_cb; }




