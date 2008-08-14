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

#include "grids/event.h"
#include "grids/define.h"

namespace Grids
{
    Event::Event()
    {
        Event( "NULL_EVENT" );
    }

    Event::Event( std::string in_event )
		: event_type( in_event ), entry_position( 0 )
    {		
	
    }

    Event::Event( std::string in_event, gridsmap_t in_map )
		: event_type( in_event ), event_map( in_map ), entry_position( 0 )
    {
	
	}

    void Event::setEvent( std::string in_event)
    {
        event_type = in_event;
    }

    void Event::setMap( gridsmap_t in_map)
    {
        event_map = in_map;
	}

    gridsmap_t Event::getMap()
	{
		return event_map;
	}
	
	gridsmap_t * Event::getMapPtr( )
	{
		return &event_map;
	}

    std::string Event::getEventType()
    {
        return event_type;
    }
	
	int Event::size()
	{
		return event_map.size();
	}
 
}

