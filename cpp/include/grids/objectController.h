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
			~ObjectController( );
		
			/// *** NOTE Create object sends the request to create an object
			// the creation actually occurs when the network bounces back a
			// GridsID, confirming the request
			void requestCreateObject( Kaleidoscope::Device *, Value ); // creates an object 
		
			void createObject( Kaleidoscope::Device *, Value ); // params, object id
		
			void requestUpdateValue( Kaleidoscope::Device *, Object *, Value );
			void requestUpdateValue( Kaleidoscope::Device *, GridsID, Value );
			
			void updateValue( Kaleidoscope::Device *, Object *, Value );
			void updateValue( Kaleidoscope::Device *, GridsID, Value );
					
			void detectSelection( Kaleidoscope::Device *, int, int );
		
			float intersectRaySphere( Vec3D, Vec3D, Vec3D, float );
			GridsID getIdFromPointer( Object * );
		
			void registerObject( GridsID, Object * );

		private:
		
			void addIdToVector( GridsID );
			
			std::vector< GridsID > object_ids;
			std::map< GridsID, Object * > id_pointer_hash;
			std::map< Object *, GridsID > pointer_id_hash;

	};

} // end namepace Grids

