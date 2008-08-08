/*
 *  personController.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "personController.h"


namespace Grids
{
	
	PersonController::PersonController( )
	{
	
	}
	
	PersonController::addPerson( Person * psn )
	{
		people.push_back( psn );
		
		psn->person_controller = &this;
	}
	
	void PersonController::setInterface( Interface * intr )
	{
		interface = intr;
	}
	
	
	PersonController::sendEvent( Event * evt )
	{
		interface->sendEvent( evt );
	
	}
	
	PersonController::giveEvent( Event * evt )
	{
		for( vector< Person * >::size_type i = 0; i < people.size(); i++ )
		{
			people[i]->giveEvent( evt );
		}
	}

} // end namespace Grids

