/*
 *  messengerController.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "messengerController.h"


namespace Grids
{
	
	MessengerController::MessengerController( )
	{
	
	}
	
	void MessengerController::addMessenger( Messenger * msg )
	{
		messengers.push_back( msg );
		
		msg->messenger_controller = &this;
		
		// Add the item's uuid to a lookup table
		
		// Public key / private key
	}
	
	void MessengerController::sendEvent( Event * evt )
	{
		interface->sendEvent( evt );
	}
	
	void MessengerController::giveEvent( Event * evt )
	{
		for( vector< Messenger * >::size_type i = 0; i < messengers.size(); i++ )
		{
			messengers[i]->giveEvent( evt );
		}
	
	}
		
} // end namespace Grids