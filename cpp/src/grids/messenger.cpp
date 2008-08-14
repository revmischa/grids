/*
 *  messenger.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <grids/messenger.h>


namespace Grids
{
	
	Messenger::Messenger( )
	{
	
	}
	
	void Messenger::giveEvent( Event * evt )
	// Called when an event is received
	{
	
	}
	
	void Messenger::sendEvent( std::string in_type, std::map< std::string, std::string > complex_value )
	{
		controller->sendEvent( in_type, complex_value );
	}
	
	void Messenger::setController( MessengerController *cont )
	{
		controller = cont;
	}

} // end namespace Grids

