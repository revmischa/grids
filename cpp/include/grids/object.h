/*
 *  GridsObject.h
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

#include <grids/event.h>
#include <grids/objectController.h>

#include <kaleidoscope/device.h>
#include <kaleidoscope/geo.h>

namespace Kaleidoscope
{
	class Device;
}

namespace Grids
{
	class ObjectController;

	class Object
	{
	public:

		Object( );
	
		void requestUpdatePosition( Kaleidoscope::Device *, Vec3D, Vec3D, Vec3D );
		void updatePosition( Kaleidoscope::Device *, Value * );
	
		void create( Kaleidoscope::Device *, Value );

		void setController( ObjectController * );
	
		static float detectSelection( Kaleidoscope::Device *, GridsID, Vec3D, Vec3D );
		
		void selectObject( Kaleidoscope::Device * );
		
		/* This should be overloaded for more complex objects (models, etc) */
		float calculateRadius( Kaleidoscope::Device *, GridsID );
	
		/* calculateRadiusFromVertices adds a [ this_id ][ "radius" ] entry to the world value LOCALLY */
		float calculateRadiusFromVertices( Kaleidoscope::Device *, GridsID );
	
		static void loadPosition( Value* temp_value, Vec3D pos, Vec3D rot, Vec3D scl );
		static void setRoom( Value*, GridsID );

	private:

		ObjectController * controller;

	};

} // end namespace Grids

