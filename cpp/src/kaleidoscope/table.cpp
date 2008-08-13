/*
 *  table.cpp
 *  kaleidoscope_2
 *
 *  Created by Patrick Tierney on 8/12/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "kaleidoscope/table.h"


namespace Kaleidoscope
{
	
	Table::Table()
	{
	
	}
	
	void Table::renderQuads( Device * d )
	{
		
		

	   // Top face; offset.  White, 50% opaque.
	 
	   glNormal3f( 0.0f, 1.0f, 0.0f);  glColor4f(0.5,0.5,0.5,.5);

	   glTexCoord2f(0.005f, 1.995f); glVertex3f(-1.0f,  1.3f, -1.0f);
	   glTexCoord2f(0.005f, 0.005f); glVertex3f(-1.0f,  1.3f,  1.0f);
	   glTexCoord2f(1.995f, 0.005f); glVertex3f( 1.0f,  1.3f,  1.0f);
	   glTexCoord2f(1.995f, 1.995f); glVertex3f( 1.0f,  1.3f, -1.0f);


			
	}
	
	
} // end namespace Kaleidoscope



