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
	class Interface; // C++ lame-ness
	
	class ObjectController
	{
		public:
			ObjectController();
			
			void setInterface( Interface * );
			
			void addObject( Object *);
			
			void sendEvent( Event *); // sends an event to the GInterface
			
			void giveEvent( Event *); // The GInterface passes messages using this
			
		private:
			Interface * controller_interface;
			
			std::map< std::string, int > uuid_map;
			
			std::vector< Object * > objects;
			
	};
	
} // end namepace Grids

