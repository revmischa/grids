/*
 *  GridsObjectController.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#pragma once

#include <grids/interface.h>
#include <grids/event.h>
#include <grids/object.h>

#include <vector>
#include <map>

namespace Grids
{
	class Interface;
	
	class ObjectController
	{
		public:
			ObjectController();
			
			void setInterface( Interface * );
			
			void addObject( Object *);
			
			void sendEvent( std::string type, std::map< std::string, std::string > );
			
			void giveEvent( Event *); // The GInterface passes messages using this
			
		private:
			Interface * controller_interface;
			
			std::map< std::string, int > uuid_map;
			
			std::vector< Object * > objects;
			
	};
	
} // end namepace Grids

