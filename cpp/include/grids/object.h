/*
 *  GridsObject.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
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
			void updatePosition( Kaleidoscope::Device *, Vec3D, Vec3D, Vec3D );
		
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

