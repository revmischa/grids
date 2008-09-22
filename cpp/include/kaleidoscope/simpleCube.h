/*
 *  simpleCube.h
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 9/15/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#pragma once

#include <kaleidoscope/device.h>
#include <grids/interface.h>
#include <grids/object.h>


namespace Grids
{
	class Interface;
	class Object;
}

namespace Kaleidoscope
{
	
	class SimpleCube : public Grids::Object
	// makes a cube in the specified room
	{
		public:
			
			// Params: Device, room, position, color_array
			// Interface generates
			SimpleCube( );//Device *, GridsID, Vec3D, float, float * );
		
			void requestCreateCube( Device *, GridsID, Vec3D, float, float * );
		
			void create( Device *, Grids::Value );
			
			float detectSelection( Device *, Vec3D, Vec3D );
			
			void selectObject( Device * d );
			
	};
	
} // end namespace Kaleidoscope
