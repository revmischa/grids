
/*
 *  GridsQueue.cpp
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
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


