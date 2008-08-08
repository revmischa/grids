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
			
			addPerson( Person * );
			
			sendEvent( Event * );
			
			giveEvent( Event * );
			
		private:
			
			Interface * interface;
			
			map< std::string, int > uuid_map;
			
			vector< Object * > people;
			
	};
	
} // end namespace Grids
