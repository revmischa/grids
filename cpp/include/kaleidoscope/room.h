/*
 *  room.h
 *  kaleidoscope_2
 *
 *  Created by Patrick Tierney on 8/11/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#pragma once

#include <grids/object.h>
#include <kaleidoscope/RenderObject.h>

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
			
			// Add on object  ID => vertices
			void addObject( Device *, std::map< std::string, std::vector< float > > );
			
		private:
		
	};

} // end namespace Kaleidoscope
