
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


#ifndef GRIDS_EVENT_INCLUDE
#define GRIDS_EVENT_INCLUDE

#include <string>
#include <map>

#include "GridsDefine.h"

namespace Grids
{

	class GEvent
	{
		public:
			GEvent( );
			GEvent( std::string );
			GEvent( std::string, gridsmap_t );

			void setEvent( std::string );
			void setHash( gridsmap_t );

			gridsmap_t getEvent();
			std::string getEventType();

		private:
			std::string event_type;
			gridsmap_t event_map;
	};

}


# endif
