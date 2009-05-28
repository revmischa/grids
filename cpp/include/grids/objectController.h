/*
 *  GridsObjectController.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
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
		void requestCreateObject( Kaleidoscope::Device *, Value * ); // creates an object 
	
		void createObject( Kaleidoscope::Device *, Value *); // params, object id
		
		void parseUpdate( Kaleidoscope::Device *, Value *);
   		void parseListRooms(  Kaleidoscope::Device * d, Value *);
				
		void requestUpdatePasition( Kaleidoscope::Device *, GridsID, GridsID, Vec3D );
		void updatePosition( Kaleidoscope::Device *, Value * );
	
		void requestUpdateValue( Kaleidoscope::Device *, Object *, Value *);
		void requestUpdateValue( Kaleidoscope::Device *, GridsID, Value * );
		
		void updateValue( Kaleidoscope::Device *, Object *, Value );
		void updateValue( Kaleidoscope::Device *, GridsID, Value );
				
		void detectSelection( Kaleidoscope::Device *, int, int );
	
		float intersectRaySphere( Vec3D, Vec3D, Vec3D, float );
		GridsID getIdFromPointer( Object * );
	
		void registerObject( GridsID, Object * );
		
		float getDistFromRay( Kaleidoscope::Device *, GridsID, Vec3D, Vec3D );
		void selectObject( Kaleidoscope::Device *, GridsID );

	private:
	
		void addIdToVector( GridsID );
		
		std::vector< GridsID > object_ids;
		std::map< GridsID, Object * > id_pointer_hash;
		std::map< Object *, GridsID > pointer_id_hash;
		
		void updateValue( Kaleidoscope::Device *, GridsID, Value * );

	};

} // end namepace Grids

