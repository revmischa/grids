/*
 *  eventController.h
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

#include <grids/define.h>
#include <kaleidoscope/device.h>


namespace Kaleidoscope
{
	class Device;
	
	class EventController
	{
		public:
			
			EventController( Device *);
			
			void setDevice( Device * );
			
			void checkEvents( Device * );
			
			void mousePressedCall( int, int, int, int );
			void mouseMovedCall( int, int );
			void mouseDraggedCall( int, int );
			
			void keyPressedCall( unsigned char, int, int );
			void specialKeyPressedCall( int, int, int );
			
			void keyUpCall( unsigned char, int, int );
			void specialKeyUpCall( int, int, int );
			
			int getMouseX();
			int getMouseY();
			
			bool keyPressed(); // Returns true if a key is held down
			bool specialKeyPressed();
			
			unsigned char key();
			int specialKey();
		
		private:
			
			Device * device;
	
	};
	
} // end namespace Kaleidoscope
