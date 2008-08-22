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

namespace Kaleidoscope
{
	class Gui
	{
		public:
			
			Gui();
			
			void drawGui( Device *);
			
			// Prepare the transparency, view, and all onther things
			// to draw on a 2D plane
			void prepareGui( Device *);
			
			// Do cleanup, etc, restore settings for 3D drawing
			void finishGui( Device *);
			
			void drawRect( irr::core::rect< int > );
			void drawRect( irr::core::rect< int >, float * );
			
			void drawRectOutline( irr::core::rect< int > );
			void drawRectOutline( irr::core::rect< int >, float * );
			
			void guiEvents( Device * );
			
			// Args: Button
			void registerClick( int, int, int );
		
		
		private:
			
	
	};
	
} // end namespace Kaleidoscope

