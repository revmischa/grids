/*
 *  person.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "person.h"


namespace Grids
{
	Person::Person( )
	{
	
	}
	
	void Person::giveEvent( Event * evt )
	// called when an event is received
	{
		
	}
	
	void sendEvent( Event * evt )
	{
		controller->sendEvent( evt );
	}

} // end namespace Grids