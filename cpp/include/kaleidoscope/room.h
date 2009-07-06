/*
 *  room.h
 *  kaleidoscope_2
 *
 *  Created by Patrick Tierney on 8/11/08.
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
 
 
 // Information should be stored in

#pragma once

#include <kaleidoscope/glSpaceObject.h>

#include <grids/define.h>

namespace Grids{
	class Object;
}

namespace Kaleidoscope
{
	class RenderObject;	
	
	class Room : public GLSpaceObject
		{
		public:
			
			Room( Device*, Grids::Value* );
			
			void draw( Device* );
			void create( Device*, Grids::Value* );

			static void requestCreateRoom( Device*, float room_size );
			
			Grids::Object* getParentFromValue( Device*, Grids::Value* );

			static void placeRoom( Device*, Room* );
			static void buildRoom( Device*, Room* );

			virtual Grids::GridsID getIDFromValue( Grids::Value* );			
		
		protected:

		private:
		
		};

} // end namespace Kaleidoscope
