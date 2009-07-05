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

#include <kaleidoscope/device.h>
#include <grids/object.h>

#include <SDL/SDL_opengl.h>

#include <iostream>

namespace Grids {
	class Object;	
}

namespace Kaleidoscope
{
	class Device;
	
	class Renderer : public Grids::Object
	{

	public:
		
		Renderer( Device *, int, int, Grids::Value* );
		~Renderer();
		
		void setDevice( Device * );
		
		void renderAll( Device *);
		void resizeScene( Device *, int, int );
							
		int getWidth( Device *);
		int getHeight( Device *);
					
		void prepare( Device *);
		//void buildTextures();
		
		static void setRenderVars( Device*, int, int );

		void draw( Device* );
		void create( Device*, Grids::Value*  );
		
		// still needed: access interface / object controller etc
		// get the positions, ect of all the objects, use that to render all
		
		void lock( Device * d);
		void unlock( Device * d );
	
	private:
		Device * device;
		
		void setWidth( Device *, int );
		void setHeight( Device *, int );
		
		static void loadLights( Device *);
		static void loadTextModes( Device *);
		
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
