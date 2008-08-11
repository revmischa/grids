/*
 *  renderer.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/9/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#pragma once

#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <kaleidoscope/device.h>

namespace Kaleidoscope
{
	class Device;
	
	class Renderer
	{

		public:
			
			Renderer( Device *, int, int );
			
			void setDevice( Device * );
			
			void renderAll();
			void resizeScene( int, int );
			
			void drawBox();
			
			void printString( void *, char *);
			
			int getWidth();
			int getHeight();
			
			void setWindowID( int );
			
			void prepare();
			void buildTextures();
			
			// still needed: access interface / object controller etc
			// get the positions, ect of all the objects, use that to render all
			
		
		private:
			Device * device;
			
			void setWidth( int );
			void setHeight( int );
			
			void loadLights();
			void loadTextModes();
			
	};
	
}
