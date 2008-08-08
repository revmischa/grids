/*
 *  personController.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
#pragma once 

#include "interface.h"
#include "event.h"
#include "person.h"
#include <vector>
#include <map>

namespace Grids
{
	class PersonController
	{
		public:
			
			PersonController();
			
			void setInterface( Interface * );
 
			
			void addPerson( Person * );
			
			void sendEvent( Event * );
			
			void giveEvent( Event * );
			
		private:
			
			Interface * interface;
			
			std::map< std::string, int > uuid_map;
			
			std::vector< Object * > people;
			
	};
	
} // end namespace Grids