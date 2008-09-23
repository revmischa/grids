/*
 *  Event.cpp
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
 */


#include <string>
#include <vector>
#include <iostream>

#include <grids/event.h>
#include <grids/define.h>

namespace Grids
{
    Event::Event()
    {
        Event( "NULL_EVENT" );
    }

    Event::Event( std::string in_event )
    {
		event_type = in_event;
    }

    Event::Event( std::string in_event, Value value )
    {
		event_type = in_event;
		args = value;
	}

    void Event::setEvent(std::string in_event)
    {
        event_type = in_event;
    }

    void Event::setArgs( Value value)
    {
        args = value;
	}

    Grids::Value Event::getArgs()
	{
		return args;
	}

	Grids::Value * Event::getArgsPtr( )
	{
		return &args;
	}

    std::string Event::getEventType()
    {
        return event_type;
    }



}

