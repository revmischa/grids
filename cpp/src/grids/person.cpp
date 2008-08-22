/*
 *  person.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <grids/person.h>


namespace Grids
{
	Person::Person( )
	{
	
	}
	
	void Person::giveEvent( Event * evt )
	// called when an event is received
	{
		
	}
	
	void Person::sendEvent(  std::string in_type, complex_type value_map  )
	{
		controller->sendEvent( in_type, value_map );
	}
	
	void Person::setController( PersonController * psn )
	{
		controller = psn;
	}

} // end namespace Grids
