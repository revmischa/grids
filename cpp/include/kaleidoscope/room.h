/*
 *  room.h
 *  kaleidoscope_2
 *
 *  Created by Patrick Tierney on 8/11/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
 
 // Information should be stored in

#pragma once

#include <grids/object.h>
#include <kaleidoscope/RenderObject.h>
#include <grids/define.h>

namespace Grids
{
	class Object;
}

namespace Kaleidoscope
{

class RenderObject;	
	class Room 
	{
		public:
			
			Room( );
			
			// Add on object  ID => vertices  to the room
			void addObject( Device *, std::map< std::string, std::vector< float > > );
			
			void moveRoom( Device *, Vec3D * );
			// Moving a room updates the position vector in the vertex hash thing
			
			void deleteRoom( Device * );
			
		private:
		
	};

} // end namespace Kaleidoscope
