/*
 *  cursorController.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#pragma once

#include <kaleidoscope/define.h> // for Vec2D
#include <kaleidoscope/device.h>

namespace Kaleidoscope
{
	class Device;
	
	class CursorController
	{
		public:
			
			CursorController( Device * );
			
			void setDevice( Device * );
			
			// 0.5f, 0.5f sets the cursor the the center of the window
			void setPosition( float, float, Device * );
			void setToCenter();
			
			Vec2D getRelativePosition( Device *);
			
			void setNotches( int );
			void getNotches( );
		
		private:
			
			Device * device;
	
	};
	
}
