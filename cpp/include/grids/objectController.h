/*
 *  GridsObjectController.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#pragma once

#include "interface.h"
#include "event.h"
#include "object.h"
#include <vector>
#include <map>

namespace Grids
{
	class ObjectController
	{
		public:
			ObjectController();
			
			addObject( Object *);
			
			sendEvent( Event *); // sends an event to the GInterface
			
			giveEvent( Event *); // The GInterface passes messages using this
			
		private:
			Interface * interface;
			
			map< std::string, int > uuid_map;
			
			vector< Object * > objects;
			
	};
	
} // end namepace Grids
