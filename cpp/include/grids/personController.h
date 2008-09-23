/*
 *  personController.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *
 *	 This file is part of Grids/Kaleidoscope.
 *	 
 *	 Grids/Kaleidoscope is free software: you can redistribute it and/or modify
 *	 it under the terms of the GNU General Public License as published by
 *	 the Free Software Foundation, either version 3 of the License, or
 *	 (at your option) any later version.
 *	 
 *	 Grids/Kaleidoscope is distributed in the hope that it will be useful,
 *	 but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	 GNU General Public License for more details.
 *	 
 *	 You should have received a copy of the GNU General Public License
 *	 along with Grids/Kaleidoscope.  If not, see <http://www.gnu.org/licenses/>.
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
