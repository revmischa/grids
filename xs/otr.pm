#!/usr/bin/perl

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

void otrInit( char * client_id, void * userstate )
{
	OtrlUserState temp_user_state = otrl_userstate_create();
}



END_OF_C_CODE

#Perl code goes here


print "loldongs\n";
$client_id = "cody";
$userstate;

otrInit( $client_id, $userstate );
