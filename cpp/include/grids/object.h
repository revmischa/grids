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
		
			float detectSelection( Kaleidoscope::Device *, Vec3D, Vec3D );
			
			void selectObject( Kaleidoscope::Device * );
		
			float calculateRadiusFromVertices( Kaleidoscope::Device *, GridsID );
		
			float distanceToSphereAlongRay( Vec3D, Vec3D, Vec3D, float );

		private:

			ObjectController * controller;

	};

} // end namespace Grids

