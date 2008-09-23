/*
 *  personController.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *
 *	 This file is part of Grids/Kaleidoscope.
 *	 
 *	 Grids/Kaleidoscope is free software: you can redistribute it and/or modify
 *	 it under the terms of the GNU General Public License as published by
 *	 the Free Software Foundation, either version 3 of the License, or
 *	 (at your option) any later version.
 *	 
 *	 Grids/Kaleidoscope is distributed in the hope that it will be useful,
 *	 but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	 GNU General Public License for more details.
 *	 
 *	 You should have received a copy of the GNU General Public License
 *	 along with Grids/Kaleidoscope.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <grids/personController.h>


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
	
	void PersonController::parseEvent( Kaleidoscope::Device * d, Event * evt )
	{
		std::string event_type = evt->getEventType();
		
		
	}
	
	void PersonController::sendUpdatePosition( Kaleidoscope::Device * d, GridsID in_id, 
											Vec3D in_pos, Vec3D in_rot, Vec3D in_scale )
	// Sends a request to Grids to update the position of a person, or that a person's position, etc has changed
	{
		
		
	}
	
	void PersonController::updatePosition( Kaleidoscope::Device * d, GridsID in_id, 
											  Vec3D in_pos, Vec3D in_rot, Vec3D in_scale )
	// Move / update a person's Value hash to reflect the new position
	// Called by parseEvent
	{
		
		
	}

	void PersonController::sendEvent( std::string in_type, Value args )

	{
		//controller_interface->sendEvent( in_type, args );

	}

	void PersonController::giveEvent( Event * evt )
	{
		for( std::vector< Person * >::size_type i = 0; i < people.size(); i++ )
		{
			people[i]->giveEvent( evt );
		}
	}

} // end namespace Grids

