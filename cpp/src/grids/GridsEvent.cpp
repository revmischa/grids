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
#include <iostream>

#include "grids/GridsEvent.h"
#include "grids/GridsDefine.h"

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
	
	}

    void GEvent::setEvent( std::string in_event)
    {
        event_type = in_event;
    }

    void GEvent::setMap( gridsmap_t in_map)
    {
        event_map = in_map;
	}

    gridsmap_t GEvent::getMap()
    {
        return event_map;
    }

    std::string GEvent::getEventType()
    {
        return event_type;
    }
	
	int GEvent::size()
	{
		return event_map.size();
	}
	
	std::vector<std::string> GEvent::pop( )
	{
		if( entry_position < size() )
		{
			std::vector<std::string> temp_vector( 2 );
			
			giterator p = event_map.begin();
			
			for( int i = 0; i < entry_position; i++ )
			{
				p++;
			}
			
			temp_vector[0] =  p->first;
			temp_vector[1] =  p->second;
			
			entry_position++;
			
			return temp_vector;
		}
	}

}

