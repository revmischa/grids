/*
 *  roomWalls.h
 *  kaleidoscope_2
 *
 *  Created by Patrick Tierney on 8/12/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
 
 #pragma once
 
 #include <kaleidoscope/RenderObject.h>
 #include <kaleidoscope/device.h>
 
 
 
 namespace Kaleidoscope
 {
	class RoomWalls : public Grids::Object, public RenderObject
	{
		public:
			
			RoomWalls();
			
			void renderQuads( Device * );
	
	};
 
 } // end namespace Kaleidoscope

