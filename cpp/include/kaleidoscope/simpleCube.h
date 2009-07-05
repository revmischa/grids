/*
 *  simpleCube.h
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 9/15/08.
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
#include <kaleidoscope/glSpaceObject.h>


namespace Grids
{
	class Interface;
	class Object;
}

namespace Kaleidoscope
{
	
	class SimpleCube : public GLSpaceObject
		// makes a cube in the specified room
		{
		public:
		
			// Params: Device, room, position, color_array
			// Interface generates
			SimpleCube( Device*, Grids::Value* );//Device *, GridsID, Vec3D, float, float * );
	
			static GridsID requestCreate( Device *, GridsID parent, Vec3D, float, float * );
	
			void create( Device *, Grids::Value *);
			void draw( Device* );
		
			static void loadGeometry( Grids::Value*, float half_side_size, float* color_array );

			float detectSelection( Device *, Vec3D, Vec3D );
		
			void selectObject( Device * d );

		private:
		
			
		};
	
} // end namespace Kaleidoscope
