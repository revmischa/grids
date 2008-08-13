/*
 *  object.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "grids/object.h"


namespace Grids
{
	
	Object::Object( )
	{
	
	}
	
	void Object::giveEvent( Event * evt)
	// Called when an event is received
	{
		
		
	}
	
	void Object::sendEvent( Event * evt )
	{
		controller->sendEvent( evt );
	}
	
	void Object::setController( ObjectController * cont )
	{
		controller = cont ;
	}



} // end namespace Grids
