/*
 *  builder.h
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 8/20/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#pragma once

#include <kaleidoscope/device.h>
#include <kaleidoscope/define.h>

namespace Kaleidoscope
{
	class Builder //  sounds like Bob the Builder or something, maybe change it...
	{
		public:
		
			Builder();
			
			void placeRoom( Device * , GridsID );	// placeRoom adds a room hash entry onto the worldHash,
												// and sets it's position based on the current number of rooms
			
			void buildRoom( Device *, GridsID );	// Constructs a generic green room ... in the future this will probably load an .obj,
													// or quake map
			
			// agrs:  device, object ID, room ID, position
			void placeObject( Device *, GridsID, GridsID, Vec3D, Vec3D, Vec3D );	// Places an object into the world hash, 
			
			void placeObject( Device *, GridsID, Vec3D );	// Places an object into the world hash, 
			
			void buildChair( Device *, GridsID ); // Adds chair quads, polygons to specific ID
			
			void buildBox( Device *, GridsID, float, float * ); // size, color[]

	};

} // end namespace Kaleidoscope

