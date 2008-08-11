/*
 *  eventController.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#pragma once

#include <GLUT/glut.h>
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
