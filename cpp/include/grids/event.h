
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
			Event( std::string, complex_type );

			void setEvent( std::string );
			void setComplexType( complex_type );
			
			complex_type getComplexType();
			complex_type * getComplexTypePointer();
			std::string getEventType();
			
									
		private:
			std::string event_type;
			complex_type stored_value;
			
	};

}


