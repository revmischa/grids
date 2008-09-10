/*
 *  GridsObjectController.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#pragma once

#include <grids/define.h>
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
			
		
			/// *** NOTE Create object sends the request to create an object
			// the creation actually occurs when the network bounces back a
			// GridsID, confirming the request
			void sendCreateObject(); // creates an object outside of a room
			void sendCreateObject( GridsID ); // creates an object inside of a room based on the ID
		
			void sendUpdatePosition( GridsID, Vec3D, Vec3D, Vec3D ); // update the position, rotation, and scale
					

			void sendEvent( std::string type, Grids::Value );

			void giveEvent( Event *); // The GInterface passes messages using this

		private:
			Interface * controller_interface;

	};

} // end namepace Grids

