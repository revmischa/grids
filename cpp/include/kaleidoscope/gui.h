/*
 *  gui.h
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 8/21/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
#pragma once

#include <kaleidoscope/device.h>
#include <kaleidoscope/rect.h>
#include <SDL_ttf/SDL_ttf.h>


namespace Kaleidoscope
{
	
	
	class Gui
	{
		public:
			
			Gui( Device * );
			
			void drawGui( Device * );
			void drawAll( Device * );
			void drawScreenText( Device * );
			
			void drawSpaceText( Device * );
			
			
			// Prepare the transparency, view, and all onther things
			// to draw on a 2D plane
			void prepareGui( Device *);
		
			void prepareText( Device * );
			void finishText( Device * );
		
			void prepareSpaceText();
			void finishSpaceText();
			
			// Do cleanup, etc, restore settings for 3D drawing
			void finishGui( Device *);
			
			void drawRect( irr::core::rect< int > );
			void drawRect( irr::core::rect< int >, float * );
			
			void drawRectOutline( irr::core::rect< int > );
			void drawRectOutline( irr::core::rect< int >, float * );
			
			void guiEvents( Device * );
			
			// Args: Button
			void registerClick( int, int, int );
		
			int addText( Device *, Vec2D, std::string );
			int addText( Device *, Vec3D, std::string );
		
			std::string getText( Device *, int, int );
			void updateText( Device *, int, int, std::string );
			void appendText( Device *, int, int, std::string );
		
			SDL_Surface * createSDLTextSurface( Device *, std::string, TTF_Font * );
			
			// camX, camY, camZ,  objPosX, Y, Z
			void billboardSphericalBegin( float, float, float, float, float, float );
			void billboardEnd();
		
			void parseText( Device *, int, int );
			void parseString( Device *, std::string );
				
			void removeLastCharacter( Device *, int, int );
			void clearText( Device *, int, int );
		
		
		private:
					
	
	};
	
} // end namespace Kaleidoscope

