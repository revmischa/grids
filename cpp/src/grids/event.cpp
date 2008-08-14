/*
 *  Event.cpp
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#include <string>
#include <map>
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
		: event_type( in_event )
    {		
	
    }

    Event::Event( std::string in_event, complex_type value )
		: event_type( in_event ), stored_value( value )
    {
	
	}

    void Event::setEvent( std::string in_event)
    {
        event_type = in_event;
    }

    void Event::setComplexType( complex_type value)
    {
        stored_value = value;
	}

    complex_type Event::getComplexType()
	{
		return stored_value;
	}
	
	complex_type * Event::getComplexTypePointer( )
	{
		return &stored_value;
	}

    std::string Event::getEventType()
    {
        return event_type;
    }
	
	
 
}

