/*
 *  GridsQueue.cpp
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#include <string>
#include <map>
#include <vector>

#include "GridsEvent.h"
#include "GridsDefine.h"

namespace Grids
{
    GEvent::GEvent()
    {
        GEvent( "NULL_EVENT" );
    }

    GEvent::GEvent( std::string in_event )
		: event_type( in_event ), entry_position( 0 )
    {		
	
    }

    GEvent::GEvent( std::string in_event, gridsmap_t in_map )
		: event_type( in_event ), event_map( in_map ), entry_position( 0 )
    {
		p = event_map.begin();
	}

    void GEvent::setEvent( std::string in_event)
    {
        event_type = in_event;
    }

    void GEvent::setMap( gridsmap_t in_map)
    {
        event_map = in_map;
		
		p = event_map.begin();
    }

    gridsmap_t GEvent::getEvent()
    {
        return event_map;
    }

    std::string GEvent::getEventType()
    {
        return event_type;
    }
	
	int GEvent::getLength()
	{
		return event_map.size();
	}
	
	std::vector<std::string> GEvent::popEntry( )
	{
		std::vector<std::string> temp_vector( 2 );
		
		temp_vector.push_back( p->first );
		temp_vector.push_back( p->second );
		
		p++;
		entry_position++;
		
		return temp_vector;
	}

}

