/*
 *  Event.cpp
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
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

