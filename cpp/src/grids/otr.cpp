

/*
 * otr.cpp
 * 
 * This module adds Off The Record encryption to Grids
 *
 */

// Issues:
// 1) How can this OTR class handle object-object encryption?
// 2) What is the difference between a user and an account name?


#include <grids/otr.h>


namespace Grids
{
	OTR::OTR( ) 
	{		
		
	}

	OTR::~OTR( ) 
	{

	}

	void OTR::init()
	{
		
	}

	// otrInint initializes a userState for a given ID
	// Every person and object that wishes to send encrypted messages needs to have a userstate
	void OTR::otrInit(  )
	{
		/*
		std::string root; // root directory, where to store the private key file
		std::string temp_key;
		std::string temp_fpr;

		GridsID client_id = otr.otr_device->getMyID();

		if( otr.otr_device->getOtrUserState() ) // If we already have a user state don't do anything
			return;

		OTRL_INIT;

		otr.otr_device->setOtrUserState( otrl_userstate_create() );
		root = otrGetDir(); // gets something like ~/ or /home/patrick/

		temp_key = root + client_id + ".key";
		temp_fpr = root + client_id + ".fpr";

		otr.otr_device->setKeyFile( temp_key );
		otr.otr_device->setFingerprintFile( temp_fpr );

		if( otrl_privkey_read( otr.otr_device->getOtrUserState(), otr.otr_device->getKeyFile().c_str() ) ) {
			std::cout << "Could not read OTR key from " << otr.otr_device->getKeyFile() << std::endl;
			otrCreatePrivKey( client_id, otr.otr_device->getProtocol() );
		}
		else {
			std::cout << "Loaded private key file from " << otr.otr_device->getKeyFile() << std::endl;
		}

		if( otrl_privkey_read_fingerprints( otr.otr_device->getOtrUserState(), otr.otr_device->getFingerprintFile().c_str(), NULL, NULL ) ) {
			std::cout << "Could not read OTR fingerprints from " << otr.otr_device->getFingerprintFile() << std::endl;
		}
		else {
			std::cout << "Loaded fingerprint file from " << otr.otr_device->getFingerprintFile() << std::endl;
		}
		*/

	}

	void OTR::otrTerminate( )
	{
		/*
		ConnContext * ctx;

		for(ctx = otr.otr_device->getOtrUserState()->context_root; ctx; ctx = ctx->next ) {
			if( ctx->msgstate == OTRL_MSGSTATE_ENCRYPTED ) {
				otrMessageDisconnect( ctx );
			}
		}
		*/
	}

	void OTR::otrMessageDisconnect( ConnContext * ctx )
	{
		/*
		if( ctx->msgstate == OTRL_MSGSTATE_ENCRYPTED ) {
			// NOTIFY Grids, etc. 
		}

		otrl_message_disconnect( otr.otr_device->getOtrUserState(), &otrOps, NULL, ctx->accountname,
							ctx->protocol, ctx->username );
		*/
	}

	std::string OTR::otrGetDir()
	{
		//return Utility::expandFilename( "~/.grids/otr/" );
	}

	void OTR::otrCreatePrivKey( std::string account_name, std::string protocol )
	{
		//cb_create_privkey( NULL, account_name.c_str(), protocol.c_str() );
	}

	void OTR::otrSend( std::string msg, GridsID target )
	{
	}



} // end namespace Grids


