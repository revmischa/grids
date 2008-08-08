
/*
 *  GridsQueue.cpp
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 *  Stores a Grids Event, ie the creation of an object, movement of an object,
 *  message, ect
 *
 */


#pragma once

#include <string>
#include <map>
#include <vector>

#include "define.h"

namespace Grids
{
	class Event
	{
		public:
			Event( );
			Event( std::string );
			Event( std::string, gridsmap_t );

			void setEvent( std::string );
			void setMap( gridsmap_t );
			
			gridsmap_t getMap();
			gridsmap_t * getMapPtr();
			std::string getEventType();
			
			int size();
			
			std::vector<std::string> pop( );

		private:
			std::string event_type;
			gridsmap_t event_map;
			
			int entry_position;
	};

}


