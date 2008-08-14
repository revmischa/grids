/*
 *  messengerController.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "grids/messengerController.h"


namespace Grids
{
	
	MessengerController::MessengerController( )
	{
	
	}
	
	void MessengerController::addMessenger( Messenger * msg )
	{
		messengers.push_back( msg );
		
		msg->setController( this );
		
		// Add the item's uuid to a lookup table
		
		// Public key / private key
	}
	
	void MessengerController::setInterface( Interface * intr )
	{
		controller_interface = intr;
	}
	
	
	void MessengerController::sendEvent( std::string in_type, std::map< std::string, std::string > complex_value)

	{
		controller_interface->sendEvent( in_type, complex_value );
	}
	
	void MessengerController::giveEvent( Event * evt )
	{
		for( std::vector< Messenger * >::size_type i = 0; i < messengers.size(); i++ )
		{
			messengers[i]->giveEvent( evt );
		}
	
	}
		
} // end namespace Grids
