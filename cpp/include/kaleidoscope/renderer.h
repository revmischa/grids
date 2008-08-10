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

namespace Kaleidoscope
{
	
	
	
	class Renderer
	{

		public:
			
			Renderer( int, int );
			
			void renderAll();
			void resizeScene( int, int );
			
			void drawBox();
			
			int getWidth();
			int getHeight();
			
			void setWindowID( int );
			
			void prepare();
			void buildTextures();
			
		
		private:
			int width;
			int height;
			
			int texture_id;
			int window_id;
						
			bool Texture_On;
			bool Light_On;
			bool Alpha_Add;
			bool Blend_On;
			bool Filtering_On;
	
	};
}
