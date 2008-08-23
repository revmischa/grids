/*
 *  renderer.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/9/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#pragma once

#include <kaleidoscope/kaleidoscope.h>

#include <SDL/SDL_opengl.h>

#include <kaleidoscope/device.h>

namespace Kaleidoscope
{
	class Device;
	
	class Renderer
	{

		public:
			
			Renderer( Device *, int, int );
			
			void setDevice( Device * );
			
			void renderAll( Device *);
			void resizeScene( Device *, int, int );
			
			void drawBox(Device *);
						
			int getWidth();
			int getHeight();
						
			void prepare( Device *);
			//void buildTextures();
			
			// still needed: access interface / object controller etc
			// get the positions, ect of all the objects, use that to render all
			
		
		private:
			Device * device;
			
			void setWidth( int );
			void setHeight( int );
			
			void loadLights( Device *);
			void loadTextModes( Device *);
			
			void prepareRender( Device *);
			void prepareQuads();
			void finishQuads();
			void prepareTriangles();
			void finishTriangles();
			void prepareLines();
			void finishLines();
			void finishRender();
			
			void renderWorldHash( Device * );
			void renderGui( Device * );
			
	};
	
}
