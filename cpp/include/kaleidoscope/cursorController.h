/*
 *  cursorController.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
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

#include <kaleidoscope/define.h> // for Vec2D
#include <kaleidoscope/device.h>

namespace Kaleidoscope
{
	class Device;
	
	class CursorController
	{
		public:
			
			CursorController( Device * );
			
			void setDevice( Device * );
			
			// 0.5f, 0.5f sets the cursor the the center of the window
			void setPosition( float, float, Device * );
			void setToCenter();
			
			Vec2D getRelativePosition( Device *);
			
			void setNotches( int );
			void getNotches( );
		
		private:
			
			Device * device;
	
	};
	
}
