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


namespace Kaleidoscope
{
	
	class EventController
	{
		public:
			
			EventController( );
			
			void mousePressedCall( int, int, int, int );
			void mouseMovedCall( int, int );
			
			void keyPressedCall( unsigned char, int, int );
			void specialKeyPressedCall( int, int, int );
			
			void keyUpCall( unsigned char, int, int );
			void specialKeyUpCall( int, int, int );
			
			int getMouseX();
			int getMouseY();
			
			bool keyPressed(); // Returns true if a key is held down
			
			unsigned char key();
			int specialKey();
		
		private:
			
			int mouseX;
			int mouseY;
			
			unsigned char stored_key;
			int special_key;
			
			int mouseButton;
			int mouseState;
			
			bool key_pressed;
	
	};
	
} // end namespace Kaleidoscope
