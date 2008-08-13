/*
 *  room.cpp
 *  kaleidoscope_2
 *
 *  Created by Patrick Tierney on 8/11/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "room.h"

namespace Kaleidoscope
{
	Room::Room()
	{
	
	}
	
	void Room::addObject( Device * d, void * user_input)
	// Can either take a Render Object, an Grids Object
	{
		// add the user input to some vector / hash that
		d->room_hash[ this ].push_back( user_input );
	}
		
		
		
} // end namespace Kaleidoscope