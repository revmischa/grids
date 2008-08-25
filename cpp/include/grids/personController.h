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

			void addPerson( Person * );

			void sendEvent( std::string , Grids::Value  );

			void giveEvent( Event * );

		private:

			Interface * controller_interface;

			std::map< std::string, int > uuid_map;

			std::vector< Person * > people;

	};

} // end namespace Grids
