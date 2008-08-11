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
			
			int width;
			int height;
			
			int current_text_mode;
			char *text_mode_string[4];
			GLint text_modes[4];
			
			int texture_id;
			int window_id;
						
			bool Texture_On;
			bool Light_On;
			bool Alpha_Add;
			bool Blend_On;
			bool Filtering_On;
			
			float Light_Ambient[4];
			float Light_Diffuse[4];
			float Light_Position[4];
			
			void loadLights();
			void loadTextModes();
			
	};
	
}
