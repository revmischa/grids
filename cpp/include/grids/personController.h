/*
 *  personController.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#pragma once

#include <grids/interface.h>
#include <grids/event.h>
#include <grids/person.h>

#include <kaleidoscope/kaleidoscope.h>

#include <vector>
#include <map>

namespace Grids
{
	class Interface;
	class Person;

	class PersonController
	{
		public:

			PersonController();

			void setInterface( Interface * );

			void sendAddPerson( );	// This should only be called once.
									// there are only one of you, right?
			
			void sendUpdatePosition( Kaleidoscope::Device *, GridsID, Vec3D, Vec3D, Vec3D );
			
			void updatePosition( Kaleidoscope::Device *, GridsID, Vec3D, Vec3D, Vec3D );
		
			void parseEvent( Kaleidoscope::Device *, Event * );

			void sendEvent( std::string, Grids::Value );

			void giveEvent( Event * );
		
			void addPerson( Person * );

		private:

			Interface * controller_interface;

			std::map< std::string, int > uuid_map;

			std::vector< Person * > people;

	};

} // end namespace Grids
