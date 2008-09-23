/*
 *  renderer.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/9/08.
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
