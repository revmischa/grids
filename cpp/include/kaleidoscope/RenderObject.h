/*
 *  RenderObject.h
 *  kaleidoscope_2
 *
 *  Created by Patrick Tierney on 8/11/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
 #pragma once
 
 #include <GL/glfw.h>
 #include <kaleidoscope/device.h>
 
 namespace Kaleidoscope
 {
	
	
	class RenderObject
	{
		public:
			
			RenderObject();
			
			void renderQuads( Device * );
			void renderTriangles( Device * );
			void renderLines( Device * );
		
		private:
		
	};
	
 } // end namespace Kaleidoscope

