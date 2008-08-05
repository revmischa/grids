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

#include "GridsEvent.h"
#include "GridsDefine.h"

namespace Grids
{
    GEvent::GEvent()
    {
        event_type = "NULL_EVENT";
    }

    GEvent::GEvent( std::string in_event )
    {
        event_type = in_event;
    }

    GEvent::GEvent( std::string in_event, gridsmap_t in_map )
    {
        event_type = in_event;
        event_map = in_map;
    }

    void GEvent::setEvent( std::string in_event)
    {
        event_type = in_event;
    }

    void GEvent::setHash( gridsmap_t in_map)
    {
        event_map = in_map;
    }

    gridsmap_t GEvent::getEvent()
    {
        return event_map;
    }

    std::string GEvent::getEventType()
    {
        return event_type;
    }

}

