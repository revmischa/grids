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

#include <kaleidoscope/device.h>

#include <vector>
#include <map>

namespace Kaleidoscope
{
	class Device;
}

namespace Grids
{
	class Interface;
	class Object;

	class ObjectController
	{
		public:
			ObjectController();
		
			/// *** NOTE Create object sends the request to create an object
			// the creation actually occurs when the network bounces back a
			// GridsID, confirming the request
			void requestCreateObject( Kaleidoscope::Device * ); // creates an object outside of a room
			void requestCreateObject( Kaleidoscope::Device *, GridsID ); // creates an object inside of a room based on the ID
		
			void createObject( Kaleidoscope::Device *, GridsID ); // params, object id
			void createObject( Kaleidoscope::Device *, GridsID, GridsID ); // params, object id, room id
			void createObject( Kaleidoscope::Device *, GridsID, Vec3D, Vec3D, Vec3D ); // params, object id
			void createObject( Kaleidoscope::Device *, GridsID, GridsID, Vec3D, Vec3D, Vec3D ); // params, object id, room id
		
			void requestUpdatePosition( Kaleidoscope::Device *, Object *, Vec3D, Vec3D, Vec3D );
			void requestUpdatePosition( Kaleidoscope::Device *, GridsID, Vec3D, Vec3D, Vec3D );
			
			void updatePosition( Kaleidoscope::Device *, Object *, Vec3D, Vec3D, Vec3D );
			void updatePosition( Kaleidoscope::Device *, GridsID, Vec3D, Vec3D, Vec3D );
		
			void sendUpdatePosition( GridsID, Vec3D, Vec3D, Vec3D ); // update the position, rotation, and scale
			
			void detectSelection( Kaleidoscope::Device *, int, int );
		
			float intersectRaySphere( Vec3D, Vec3D, Vec3D, float );
			GridsID getIdFromPointer( Object * );

		private:
		
			void addIdToVector( GridsID );
			
			std::vector< GridsID > object_ids;
			std::map< GridsID, Object * > id_pointer_hash;
			std::map< Object *, GridsID > pointer_id_hash;

	};

} // end namepace Grids

