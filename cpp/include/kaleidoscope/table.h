/*
 *  table.h
 *  kaleidoscope_2
 *
 *  Created by Patrick Tierney on 8/12/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
 #include <kaleidoscope/RenderObject.h>
 
 namespace Kaleidoscope
 {
	class RenderObject;
	
	class Table : public RenderObject, public Grids::Object
	{
		public:
			
			Table();
			
			void renderQuads( Device * );
			
		private:
			
	};
 
 
 }

