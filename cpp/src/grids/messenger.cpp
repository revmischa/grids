/*
 *  messenger.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "messenger.h"


namespace Grids
{
	
	Messenger::Messenger( )
	{
	
	}
	
	void Messenger::giveEvent( Event * evt )
	// Called when an event is received
	{
	
	}
	
	void Messenger::sendEvent( Event * evt )
	{
		controller->sendEvent( evt );
	}


} // end namespace Grids
