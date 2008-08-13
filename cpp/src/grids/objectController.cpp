/*
 *  objectController.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "grids/objectController.h"


namespace Grids
{
	ObjectController::ObjectController( )
	{
		
	}
	
	void ObjectController::addObject( Object * obj ) 
	{
		objects.push_back( obj );
		
		obj->setController( this );
		
		// Add the item's uuid to a lookup table
		
		// Public key / private key
	}
	
	void ObjectController::setInterface( Interface * intr )
	{
		controller_interface = intr;
	}
	
	void ObjectController::sendEvent( Event * evt )
	{
		controller_interface->sendEvent( evt );
	}
	
	void ObjectController::giveEvent( Event * evt )
	{
		for( std::vector< Object * >::size_type i = 0; i < objects.size(); i++ )
		{
			objects[i]->giveEvent( evt );
		}
	}
	
} // end namespace Grids

