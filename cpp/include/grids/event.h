
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

#include <grids/define.h>


namespace Grids
{
	class Event
	{
		public:
			Event( );
			Event( std::string );
			Event( std::string, Grids::Value );

			void setEvent( std::string );
			void setArgs( Grids::Value );

			Grids::Value getArgs();
			Grids::Value *getArgsPtr();
			std::string getEventType();


		private:
			std::string event_type;
			Grids::Value args;

	};

}


