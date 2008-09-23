/*
 *  RenderObject.cpp
 *  kaleidoscope_2
 *
 *  Created by Patrick Tierney on 8/11/08.
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

#include <kaleidoscope/RenderObject.h>



namespace Kaleidoscope
{
	
	RenderObject::RenderObject()
	{
	
	}
	
	void RenderObject::renderQuads( Device * d )
	{
		//Bottom Face.  Red, 75% opaque, magnified texture
	 
	   glNormal3f( 0.0f, -1.0f, 0.0f); // Needed for lighting
	   glColor4f(0.9,0.2,0.2,.75); // Basic polygon color

	   glTexCoord2f(0.800f, 0.800f); glVertex3f(-1.0f, -1.0f, -1.0f); 
	   glTexCoord2f(0.200f, 0.800f); glVertex3f( 1.0f, -1.0f, -1.0f);
	   glTexCoord2f(0.200f, 0.200f); glVertex3f( 1.0f, -1.0f,  1.0f);
	   glTexCoord2f(0.800f, 0.200f); glVertex3f(-1.0f, -1.0f,  1.0f);


	   // Top face; offset.  White, 50% opaque.
	 
	   glNormal3f( 0.0f, 1.0f, 0.0f);  glColor4f(0.5,0.5,0.5,.5);

	   glTexCoord2f(0.005f, 1.995f); glVertex3f(-1.0f,  1.3f, -1.0f);
	   glTexCoord2f(0.005f, 0.005f); glVertex3f(-1.0f,  1.3f,  1.0f);
	   glTexCoord2f(1.995f, 0.005f); glVertex3f( 1.0f,  1.3f,  1.0f);
	   glTexCoord2f(1.995f, 1.995f); glVertex3f( 1.0f,  1.3f, -1.0f);


	   // Far face.  Green, 50% opaque, non-uniform texture cooridinates.

	   glNormal3f( 0.0f, 0.0f,-1.0f);  glColor4f(0.2,0.9,0.2,.5); 

	   glTexCoord2f(0.995f, 0.005f); glVertex3f(-1.0f, -1.0f, -1.3f);
	   glTexCoord2f(2.995f, 2.995f); glVertex3f(-1.0f,  1.0f, -1.3f);
	   glTexCoord2f(0.005f, 0.995f); glVertex3f( 1.0f,  1.0f, -1.3f);
	   glTexCoord2f(0.005f, 0.005f); glVertex3f( 1.0f, -1.0f, -1.3f);


	   // Right face.  Blue; 25% opaque
	   
	   glNormal3f( 1.0f, 0.0f, 0.0f);  glColor4f(0.2,0.2,0.9,.25);

	   glTexCoord2f(0.995f, 0.005f); glVertex3f( 1.0f, -1.0f, -1.0f); 
	   glTexCoord2f(0.995f, 0.995f); glVertex3f( 1.0f,  1.0f, -1.0f);
	   glTexCoord2f(0.005f, 0.995f); glVertex3f( 1.0f,  1.0f,  1.0f);
	   glTexCoord2f(0.005f, 0.005f); glVertex3f( 1.0f, -1.0f,  1.0f);


	   // Front face; offset.  Multi-colored, 50% opaque.

	   glNormal3f( 0.0f, 0.0f, 1.0f); 

	   glColor4f( 0.9f, 0.2f, 0.2f, 0.5f);
	   glTexCoord2f( 0.005f, 0.005f); glVertex3f(-1.0f, -1.0f,  1.3f);
	   glColor4f( 0.2f, 0.9f, 0.2f, 0.5f);
	   glTexCoord2f( 0.995f, 0.005f); glVertex3f( 1.0f, -1.0f,  1.3f);
	   glColor4f( 0.2f, 0.2f, 0.9f, 0.5f);
	   glTexCoord2f( 0.995f, 0.995f); glVertex3f( 1.0f,  1.0f,  1.3f); 
	   glColor4f( 0.1f, 0.1f, 0.1f, 0.5f);
	   glTexCoord2f( 0.005f, 0.995f); glVertex3f(-1.0f,  1.0f,  1.3f);


	   // Left Face; offset.  Yellow, varying levels of opaque.
	   
	   glNormal3f(-1.0f, 0.0f, 0.0f);  
	   
	   glColor4f(0.9,0.9,0.2,0.0);
	   glTexCoord2f(0.005f, 0.005f); glVertex3f(-1.3f, -1.0f, -1.0f); 
	   glColor4f(0.9,0.9,0.2,0.66);
	   glTexCoord2f(0.995f, 0.005f); glVertex3f(-1.3f, -1.0f,  1.0f);
	   glColor4f(0.9,0.9,0.2,1.0);
	   glTexCoord2f(0.995f, 0.995f); glVertex3f(-1.3f,  1.0f,  1.0f);
	   glColor4f(0.9,0.9,0.2,0.33);
	   glTexCoord2f(0.005f, 0.995f); glVertex3f(-1.3f,  1.0f, -1.0f);
		
		
	}
	
	void RenderObject::renderTriangles( Device * d)
	{
	
	}
	
	void RenderObject::renderLines( Device * d )
	{
	
	}

}
