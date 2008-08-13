/*
 *  personController.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "grids/personController.h"


namespace Grids
{
	
	PersonController::PersonController( )
	{
	
	}
	
	void PersonController::addPerson( Person * psn )
	{
		people.push_back( psn );
		
		psn->setController( this );
	}
	
	void PersonController::setInterface( Interface * intr )
	{
		controller_interface = intr;
	}
	
	
	void PersonController::sendEvent( Event * evt )
	{
		controller_interface->sendEvent( evt );
	
	}
	
	void PersonController::giveEvent( Event * evt )
	{
		for( std::vector< Person * >::size_type i = 0; i < people.size(); i++ )
		{
			people[i]->giveEvent( evt );
		}
	}

} // end namespace Grids

