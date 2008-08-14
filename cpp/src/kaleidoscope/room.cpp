/*
 *  room.cpp
 *  kaleidoscope_2
 *
 *  Created by Patrick Tierney on 8/11/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <kaleidoscope/room.h>

namespace Kaleidoscope
{
	Room::Room()
	{
	
	}
	
	void Room::addObject( Device * d, std::map< RenderObject *, std::vector< float > > user_input )
	// Object should be in the form std::map< RenderObject *, std::vector< float > >
	// where the floats
	{
		d->vertex_hash[ this ] =  user_input ;
	}
	
	
		
		
		
} // end namespace Kaleidoscope
