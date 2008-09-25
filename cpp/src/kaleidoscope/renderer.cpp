/*
 *  renderer.cpp
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

#include <kaleidoscope/renderer.h>
#include <kaleidoscope/RenderObject.h>

namespace Kaleidoscope
{
	Renderer::Renderer( Device * d, int window_width, int window_height)
	{		
		d->width = window_width;
		d->height = window_height;
		
		d->Texture_On = false;
		d->Light_On = false;
		d->Alpha_Add = false;
		d->Blend_On = true;
		d->Filtering_On = false;
		d->Smooth_On = true;
		
		loadLights(d);
		loadTextModes(d);
	}
	
	void Renderer::setDevice( Device * in_device )
	{
		device = in_device;
	}
	
	
	void Renderer::drawBox( Device * d)
	{
		char buf[80]; // For our strings.

		if (d->Texture_On)
		  glEnable(GL_TEXTURE_2D);
	   else
		  glDisable(GL_TEXTURE_2D);

	   if (d->Light_On) 
		  glEnable(GL_LIGHTING);
	   else 
		  glDisable(GL_LIGHTING);

		if (d->Alpha_Add)
		   glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
		else
		   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		// If we're blending, we don't want z-buffering.
		if (d->Blend_On)
		   glDisable(GL_DEPTH_TEST); 
		else
		   glEnable(GL_DEPTH_TEST); 

		if (d->Filtering_On) {
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
											   GL_LINEAR_MIPMAP_LINEAR);
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		} else {
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
											   GL_NEAREST_MIPMAP_NEAREST);
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		}


	   // Need to manipulate the ModelView matrix to move our model around.
	   glMatrixMode(GL_MODELVIEW);

	   // Reset to 0,0,0; no rotation, no scaling.
	   glLoadIdentity(); 
	   
		d->getCamera()->callgluLookAt( d );


	   // Move the object back from the screen.
	   //glTranslatef(0.0f,0.0f, -5.0f);
	   
	   
	   // Rotate the calculated amount.
	   //glRotatef(X_Rot,1.0f,0.0f,0.0f);
	   //glRotatef(Y_Rot,0.0f,1.0f,0.0f);


	   // Clear the color and depth buffers.
	   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	   // OK, let's start drawing our planer quads.
	   glBegin(GL_QUADS); 


	   // Bottom Face.  Red, 75% opaque, magnified texture
	 
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


	   // All polygons have been drawn.
	   glEnd();
	   
		glBegin(GL_LINES); 

		glColor4f(0.9,0.9,1.0,1.0);
		glVertex3f( -10000, 0, 0);
		glVertex3f( 10000, 0, 0 );
		glVertex3f( 0, -10000, 0);
		glVertex3f(0, 10000, 0 );
		glVertex3f( 0, 0, -10000 );
		glVertex3f( 0, 0, 10000 );

		glEnd();
	   
	   

	   // Move back to the origin (for the text, below).
	   glLoadIdentity();

	   // We need to change the projection matrix for the text rendering.  
	   glMatrixMode(GL_PROJECTION);

	   // But we like our current view too; so we save it here.
	   glPushMatrix();

	   // Now we set up a new projection for the text.
	   glLoadIdentity();
	   glOrtho(0, d->width ,0, d->height, -1.0, 1.0);

	   // Lit or textured text looks awful.
	   glDisable(GL_TEXTURE_2D);
	   glDisable(GL_LIGHTING);

	   // We don't want depth-testing either.
	   glDisable(GL_DEPTH_TEST); 

	   // But, for fun, let's make the text partially transparent too.
	   glColor4f(0.6,1.0,0.6,.75);

	   // To ease, simply translate up.  Note we're working in screen
	   // pixels in this projection.
	   
	   glTranslatef(6.0f, d->height - 14,0.0f);

	   // Make sure we can read the FPS section by first placing a 
	   // dark, mostly opaque backdrop rectangle.
	   glColor4f(0.2,0.2,0.2,0.75);

	   glBegin(GL_QUADS);
	   glVertex3f(  0.0f, -2.0f, 0.0f);
	   glVertex3f(  0.0f, 12.0f, 0.0f);
	   glVertex3f(140.0f, 12.0f, 0.0f);
	   glVertex3f(140.0f, -2.0f, 0.0f);
	   glEnd();

	   //glColor4f(0.9,0.2,0.2,.75);
	//   sprintf(buf,"FPS: %f F: %2d", FrameRate, FrameCount);
	//   glRasterPos2i(6,0);
	//  // ourPrintString(GLUT_BITMAP_HELVETICA_12,buf);

	   // Done with this special projection matrix.  Throw it away.
	   glPopMatrix();

	   // All done drawing.  Let's show it.
	 //  glutSwapBuffers();

	   // Now let's do the motion calculations.
	  // X_Rot+=X_Speed; 
	//   Y_Rot+=Y_Speed; 

	}
	
	int Renderer::getWidth()
	{
		return device->width;
	}
	
	int Renderer::getHeight()
	{
		return device->height;
	}
	
	void Renderer::setWidth(int in_width )
	{
		device->width = in_width;
	}
	
	void Renderer::setHeight( int in_height )
	{
		device->height = in_height;
	}
	
	void Renderer::renderAll( Device * d)
	{		
		prepareRender( d );
				
		renderWorldHash( d );	
		
		renderGui( d );
				
		finishRender();
	}
	
	void Renderer::prepareRender( Device * d)
	{
		if (d->Texture_On)
		  glEnable(GL_TEXTURE_2D);
	   else
		  glDisable(GL_TEXTURE_2D);

	   if (d->Light_On) 
		  glEnable(GL_LIGHTING);
	   else 
		  glDisable(GL_LIGHTING);

		if (d->Alpha_Add)
		   glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
		else
		   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		// If we're blending, we don't want z-buffering.
		if (d->Blend_On)
		   glDisable(GL_DEPTH_TEST); 
		else
		   glEnable(GL_DEPTH_TEST); 

		if (d->Filtering_On) {
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
											   GL_LINEAR_MIPMAP_LINEAR);
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		} else {
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
											   GL_NEAREST_MIPMAP_NEAREST);
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		}
		
		if ( d->Smooth_On )
		{
			glHint( GL_LINE_SMOOTH_HINT, GL_NICEST);
			glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
		}

	   // Need to manipulate the ModelView matrix to move our model around.
	   glMatrixMode(GL_MODELVIEW);

	   // Reset to 0,0,0; no rotation, no scaling.
	   glLoadIdentity(); 
	   
		d->getCamera()->callgluLookAt( d );

	   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	}
	
	void Renderer::renderWorldHash( Device * d )
	{
		for( int i = 0; i < d->world_hash[ "rooms" ].size(); i++)
		{
			glPushMatrix();
			
			Grids::GridsID temp_room = d->world_hash[ "rooms" ][ i ].asString();
			
			// Translate
			glTranslatef(	d->world_hash[ temp_room ][ "position" ][ 0u ].asDouble(), 
							d->world_hash[ temp_room ][ "position" ][ 1u ].asDouble(), 
							d->world_hash[ temp_room ][ "position" ][ 2u ].asDouble()	);
			
			//Rotate the calculated amount.
			glRotatef( d->world_hash[ temp_room ][ "rotation" ][ 0u ].asDouble() ,1.0f,0.0f,0.0f);
			glRotatef( d->world_hash[ temp_room ][ "rotation" ][ 1u ].asDouble() ,0.0f,1.0f,0.0f);
			glRotatef( d->world_hash[ temp_room ][ "rotation" ][ 2u ].asDouble() ,0.0f,0.0f,1.0f);
			
			// Scale
			glScalef(	d->world_hash[ temp_room ][ "scale" ][ 0u ].asDouble(), 
						d->world_hash[ temp_room ][ "scale" ][ 1u ].asDouble(), 
						d->world_hash[ temp_room ][ "scale" ][ 2u ].asDouble()		); 
			
			
			// Draw Room Lines
			if( !( d->world_hash[ temp_room ][ "lines" ] ) == false )
			{
				glBegin( GL_LINES );
				
				for( int g = 0; g < d->world_hash[ temp_room ][ "lines" ].size(); g++)
				{
					if( !( d->world_hash[ temp_room][ "lines" ][ g ][ "color" ] ) == false )
					{
						int color_index = d->world_hash[ temp_room][ "lines" ][ g ][ "color" ].asInt();
						
						glColor4f(	d->world_hash[ temp_room][ "color" ][  color_index  ][ 0u ].asDouble(), 
									d->world_hash[ temp_room][ "color" ][  color_index  ][ 1u ].asDouble(), 
									d->world_hash[ temp_room][ "color" ][  color_index  ][ 2u ].asDouble(), 
									d->world_hash[ temp_room][ "color" ][  color_index  ][ 3u ].asDouble()   ); 
					}
					
					int vertice_index_1;
					int vertice_index_2;
					
					for( int h = 0; h < d->world_hash[ temp_room ][ "lines" ][ g ][ "indices"].size(); h++ )
					{
						vertice_index_1 = d->world_hash[ temp_room ][ "lines" ][ g ][ "indices"][ h ][ 0u ].asInt();
						vertice_index_2 = d->world_hash[ temp_room ][ "lines" ][ g ][ "indices"][ h ][ 1u ].asInt();
						
						glVertex3f(		d->world_hash[ temp_room ][ "vertices" ][ vertice_index_1 ][ 0u ].asDouble(), 
										d->world_hash[ temp_room ][ "vertices" ][ vertice_index_1 ][ 1u ].asDouble(), 
										d->world_hash[ temp_room ][ "vertices" ][ vertice_index_1 ][ 2u ].asDouble()	);
										
						glVertex3f(		d->world_hash[ temp_room ][ "vertices" ][ vertice_index_2 ][ 0u ].asDouble(), 
										d->world_hash[ temp_room ][ "vertices" ][ vertice_index_2 ][ 1u ].asDouble(), 
										d->world_hash[ temp_room ][ "vertices" ][ vertice_index_2 ][ 2u ].asDouble()	);
					}
				}
				
				glEnd();
			}
			
			// Draw Room Quads
			if( !( d->world_hash[ temp_room ][ "quads" ] ) == false )
			{
				glBegin( GL_QUADS );
				
				for( int g = 0; g < d->world_hash[ temp_room ][ "quads" ].size(); g++)
				{
					if( !( d->world_hash[ temp_room][ "quads" ][ g ][ "color" ] ) == false )
					{
						int color_index = d->world_hash[ temp_room][ "quads" ][ g ][ "color" ].asInt();
						
						glColor4f(	d->world_hash[ temp_room ][ "color" ][ color_index ][ 0u ].asDouble(), 
									d->world_hash[ temp_room ][ "color" ][ color_index ][ 1u ].asDouble(), 
									d->world_hash[ temp_room ][ "color" ][ color_index ][ 2u ].asDouble(), 
									d->world_hash[ temp_room ][ "color" ][ color_index ][ 3u ].asDouble()	); 
					}
					
					int vertice_index_1;
					int vertice_index_2;
					int vertice_index_3;
					int vertice_index_4;
					
					for( int h = 0; h < d->world_hash[ temp_room ][ "quads" ][ g ][ "indices" ].size(); h++ )
					{
						vertice_index_1 = d->world_hash[ temp_room ][ "quads" ][ g ][ "indices" ][ h ][ 0u ].asInt();
						vertice_index_2 = d->world_hash[ temp_room ][ "quads" ][ g ][ "indices" ][ h ][ 1u ].asInt();
						vertice_index_3 = d->world_hash[ temp_room ][ "quads" ][ g ][ "indices" ][ h ][ 2u ].asInt();
						vertice_index_4 = d->world_hash[ temp_room ][ "quads" ][ g ][ "indices" ][ h ][ 3u ].asInt();
						
						
						
						glVertex3f(	d->world_hash[ temp_room ][ "vertices" ][ vertice_index_1 ][ 0u ].asDouble(), 
									d->world_hash[ temp_room ][ "vertices" ][ vertice_index_1 ][ 1u ].asDouble(), 
									d->world_hash[ temp_room ][ "vertices" ][ vertice_index_1 ][ 2u ].asDouble()	);
									
						glVertex3f(	d->world_hash[ temp_room ][ "vertices" ][ vertice_index_2 ][ 0u ].asDouble(), 
									d->world_hash[ temp_room ][ "vertices" ][ vertice_index_2 ][ 1u ].asDouble(), 
									d->world_hash[ temp_room ][ "vertices" ][ vertice_index_2 ][ 2u ].asDouble()	);
						
						glVertex3f(	d->world_hash[ temp_room ][ "vertices" ][ vertice_index_3 ][ 0u ].asDouble(), 
									d->world_hash[ temp_room ][ "vertices" ][ vertice_index_3 ][ 1u ].asDouble(), 
									d->world_hash[ temp_room ][ "vertices" ][ vertice_index_3 ][ 2u ].asDouble()	);
									
						glVertex3f(	d->world_hash[ temp_room ][ "vertices" ][ vertice_index_4 ][ 0u ].asDouble(), 
									d->world_hash[ temp_room ][ "vertices" ][ vertice_index_4 ][ 1u ].asDouble(), 
									d->world_hash[ temp_room ][ "vertices" ][ vertice_index_4 ][ 2u ].asDouble()	);									
					} // end for h
				} // end for all quads in room
				
				glEnd();
			} // end if [ Quads ] exists
			
			
			// Draw each object in the room, keeping the translation, scale
			
			if( !(d->world_hash[ temp_room ][ "objects" ]) == false )
			{
								
				for( int g = 0; g < d->world_hash[ temp_room ][ "objects" ].size(); g++ )
				{
					GridsID temp_object =  d->world_hash[ temp_room ][ "objects" ][ g ].asString();
					
					
					glPushMatrix();
					
					// Translate
					glTranslatef(	d->world_hash[ temp_object ][ "position" ][ 0u ].asDouble(), 
									d->world_hash[ temp_object ][ "position" ][ 1u ].asDouble(), 
									d->world_hash[ temp_object ][ "position" ][ 2u ].asDouble()	);
					
					
					//Rotate the calculated amount.
					glRotatef(  d->world_hash[ temp_object ][ "rotation" ][ 0u ].asDouble() , 1.0f,0.0f,0.0f);
					glRotatef( d->world_hash[ temp_object ][ "rotation" ][ 1u ].asDouble() , 0.0f,1.0f,0.0f);
					glRotatef( d->world_hash[ temp_object ][ "rotation" ][ 2u ].asDouble() , 0.0f,0.0f,1.0f);
					
					// Scale
					glScalef(	d->world_hash[ temp_object ][ "scale" ][ 0u ].asDouble(), 
								d->world_hash[ temp_object ][ "scale" ][ 1u ].asDouble(), 
								d->world_hash[ temp_object ][ "scale" ][ 2u ].asDouble()	); 
					
									
					if( !( d->world_hash[ temp_object ][ "quads" ] ) == false ) // if there are quads
					{
						prepareQuads();
						
						
						
						for( int h = 0; h < d->world_hash[ temp_object ][ "quads" ].size(); h++ )
						{
							if( !( d->world_hash[ temp_object ][ "quads" ][ h ][ "color" ] ) == false )
							{
								int color_index = d->world_hash[ temp_object ][ "quads" ][ h ][ "color" ].asInt();
						
								glColor4f(	d->world_hash[ temp_object ][ "color" ][ color_index ][ 0u ].asDouble(), 
											d->world_hash[ temp_object ][ "color" ][ color_index ][ 1u ].asDouble(), 
											d->world_hash[ temp_object ][ "color" ][ color_index ][ 2u ].asDouble(), 
											d->world_hash[ temp_object ][ "color" ][ color_index ][ 3u ].asDouble()	);
							}
							
							int vertice_index_1;
							int vertice_index_2;
							int vertice_index_3;
							int vertice_index_4;
							
							for( int k = 0; k < d->world_hash[ temp_object ][ "quads" ][ h ][ "indices" ].size(); k++ )
							{
								vertice_index_1 = d->world_hash[ temp_object ][ "quads" ][ h ][ "indices" ][ k ][ 0u ].asInt();
								vertice_index_2 = d->world_hash[ temp_object ][ "quads" ][ h ][ "indices" ][ k ][ 1u ].asInt();
								vertice_index_3 = d->world_hash[ temp_object ][ "quads" ][ h ][ "indices" ][ k ][ 2u ].asInt();
								vertice_index_4 = d->world_hash[ temp_object ][ "quads" ][ h ][ "indices" ][ k ][ 3u ].asInt();
								
								
//								std::cout << vertice_index_1 << " " << vertice_index_2 << " " << vertice_index_3 << " " << vertice_index_4 << std::endl;
//								
//								std::cout << d->world_hash[ temp_object ][ "vertices" ][ vertice_index_1 ][ 0u ].asDouble()
//								<< " " << d->world_hash[ temp_object ][ "vertices" ][ vertice_index_1 ][ 1u ].asDouble()
//								<< " " << d->world_hash[ temp_object ][ "vertices" ][ vertice_index_1 ][ 2u ].asDouble() << std::endl;
//								
//								std::cout << d->world_hash[ temp_object ][ "vertices" ][ vertice_index_2 ][ 0u ].asDouble()
//								<< " " << d->world_hash[ temp_object ][ "vertices" ][ vertice_index_2 ][ 1u ].asDouble()
//								<< " " << d->world_hash[ temp_object ][ "vertices" ][ vertice_index_2 ][ 2u ].asDouble() << std::endl;
//								
//								std::cout << d->world_hash[ temp_object ][ "vertices" ][ vertice_index_3 ][ 0u ].asDouble()
//								<< " " << d->world_hash[ temp_object ][ "vertices" ][ vertice_index_3 ][ 1u ].asDouble()
//								<< " " << d->world_hash[ temp_object ][ "vertices" ][ vertice_index_3 ][ 2u ].asDouble() << std::endl;
//								
//								std::cout << d->world_hash[ temp_object ][ "vertices" ][ vertice_index_4 ][ 0u ].asDouble()
//								<< " " << d->world_hash[ temp_object ][ "vertices" ][ vertice_index_4 ][ 1u ].asDouble()
//								<< " " << d->world_hash[ temp_object ][ "vertices" ][ vertice_index_4 ][ 2u ].asDouble() << std::endl;
								
								
								glVertex3f( d->world_hash[ temp_object ][ "vertices" ][ vertice_index_1 ][ 0u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_1 ][ 1u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_1 ][ 2u ].asDouble()	);
											
								glVertex3f( d->world_hash[ temp_object ][ "vertices" ][ vertice_index_2 ][ 0u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_2 ][ 1u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_2 ][ 2u ].asDouble()	);
											
								glVertex3f( d->world_hash[ temp_object ][ "vertices" ][ vertice_index_3 ][ 0u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_3 ][ 1u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_3 ][ 2u ].asDouble()	);
											
								glVertex3f( d->world_hash[ temp_object ][ "vertices" ][ vertice_index_4 ][ 0u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_4 ][ 1u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_4 ][ 2u ].asDouble()	);
							
							
							} // end for each indice in a quad
								
						} // end for h -- for each quad
						
						finishQuads();
						
					} // end if Quads exists
					
					
					/////////////
					// Triangles
					////////////
					
					
					if( !( d->world_hash[ temp_object ][ "triangles" ] ) == false ) // if there are quads
					{
						prepareTriangles();
						
						for( int h = 0; h < d->world_hash[ temp_object ][ "triangles" ].size(); h++ )
						{
							if( !( d->world_hash[ temp_object ][ "triangles" ][ h ][ "color" ] ) == false )
							{
								int color_index = d->world_hash[ temp_object ][ "triangles" ][ h ][ "color" ].asInt();
						
								glColor4f(	d->world_hash[ temp_object ][ "color" ][ color_index ][ 0u ].asDouble(), 
											d->world_hash[ temp_object ][ "color" ][ color_index ][ 1u ].asDouble(), 
											d->world_hash[ temp_object ][ "color" ][ color_index ][ 2u ].asDouble(), 
											d->world_hash[ temp_object ][ "color" ][ color_index ][ 3u ].asDouble()	);
							}
							
							int vertice_index_1;
							int vertice_index_2;
							int vertice_index_3;
							
							for( int k = 0; k < d->world_hash[ temp_object ][ "triangles" ][ h ][ "indices" ].size(); k++ )
							{
								vertice_index_1 = d->world_hash[ temp_object ][ "triangles" ][ h ][ "indices" ][ k ][ 0u ].asInt();
								vertice_index_2 = d->world_hash[ temp_object ][ "triangles" ][ h ][ "indices" ][ k ][ 1u ].asInt();
								vertice_index_3 = d->world_hash[ temp_object ][ "triangles" ][ h ][ "indices" ][ k ][ 2u ].asInt();
								
								glVertex3f( d->world_hash[ temp_object ][ "vertices" ][ vertice_index_1 ][ 0u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_1 ][ 1u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_1 ][ 2u ].asDouble()	);
											
								glVertex3f( d->world_hash[ temp_object ][ "vertices" ][ vertice_index_2 ][ 0u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_2 ][ 1u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_2 ][ 2u ].asDouble()	);
											
								glVertex3f( d->world_hash[ temp_object ][ "vertices" ][ vertice_index_3 ][ 0u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_3 ][ 1u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_3 ][ 2u ].asDouble()	);
											
							
							
							} // end for each indice in a triangle
								
						} // end for h -- for each triangle
						
						finishTriangles();
						
					} // end if Triangles exists
					
					
					
					/////////////
					// Lines
					////////////
					
					
					if( !( d->world_hash[ temp_object ][ "lines" ] ) == false ) // if there are quads
					{
						prepareLines();
						
						for( int h = 0; h < d->world_hash[ temp_object ][ "lines" ].size(); h++ )
						{
							if( !( d->world_hash[ temp_object ][ "lines" ][ h ][ "color" ] ) == false )
							{
								int color_index = d->world_hash[ temp_object ][ "lines" ][ h ][ "color" ].asInt();
						
								glColor4f(	d->world_hash[ temp_object ][ "color" ][ color_index ][ 0u ].asDouble(), 
											d->world_hash[ temp_object ][ "color" ][ color_index ][ 1u ].asDouble(), 
											d->world_hash[ temp_object ][ "color" ][ color_index ][ 2u ].asDouble(), 
											d->world_hash[ temp_object ][ "color" ][ color_index ][ 3u ].asDouble()	);
							}
							
							int vertice_index_1;
							int vertice_index_2;
							
							for( int k = 0; k < d->world_hash[ temp_object ][ "lines" ][ h ][ "indices" ].size(); k++ )
							{
								vertice_index_1 = d->world_hash[ temp_object ][ "lines" ][ h ][ "indices" ][ k ][ 0u ].asInt();
								vertice_index_2 = d->world_hash[ temp_object ][ "lines" ][ h ][ "indices" ][ k ][ 1u ].asInt();
								
								glVertex3f( d->world_hash[ temp_object ][ "vertices" ][ vertice_index_1 ][ 0u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_1 ][ 1u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_1 ][ 2u ].asDouble()	);
											
								glVertex3f( d->world_hash[ temp_object ][ "vertices" ][ vertice_index_2 ][ 0u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_2 ][ 1u ].asDouble(),
											d->world_hash[ temp_object ][ "vertices" ][ vertice_index_2 ][ 2u ].asDouble()	);
	
							} // end for each indice in a line
								
						} // end for h -- for each line
						
						finishLines();
						
					} // end if Triangles exists
					
					glPopMatrix();

				} // end for g
				
			} // end if Objects
			
			glPopMatrix();
		} // end for i -- per room loop

	
	
	}
	
	void Renderer::prepareQuads()
	{
		glBegin(GL_QUADS); 
	}
	
	void Renderer::finishQuads()
	{
		glEnd();
	}
	
	void Renderer::prepareTriangles()
	{
		glBegin(GL_TRIANGLES);
	}
	
	void Renderer::finishTriangles()
	{
		glEnd();
	}
	
	void Renderer::prepareLines()
	{
		glBegin(GL_LINES);
	}
	
	void Renderer::finishLines()
	{
		glEnd();
	}
	
		
	
	void Renderer::finishRender()
	{
	
	}
	
	void Renderer::renderGui( Device * d )
	{
		//d->getGui()->prepareGui( d );
		//d->getGui()->drawGui( d );
		//d->getGui()->finishGui( d );
		d->getGui()->drawAll( d );
	}
	
	
	void Renderer::prepare( Device * d)
	{
		//buildTextures();   

	   // Color to clear color buffer to.
	   glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	   // Depth to clear depth buffer to; type of test.
	   glClearDepth(1.0);
	   glDepthFunc(GL_LESS); 

	   // Enables Smooth Color Shading; try GL_FLAT for (lack of) fun.
	   glShadeModel(GL_SMOOTH);
	   
		glEnable(GL_BLEND);

	   // Load up the correct perspective matrix; using a callback directly.
	   resizeScene(d, d->width , d->height);

	   // Set up a light, turn it on.
	   glLightfv(GL_LIGHT1, GL_POSITION, d->Light_Position);
	   glLightfv(GL_LIGHT1, GL_AMBIENT,  d->Light_Ambient);
	   glLightfv(GL_LIGHT1, GL_DIFFUSE,  d->Light_Diffuse); 
	   glEnable (GL_LIGHT1); 

	   // A handy trick -- have surface material mirror the color.
	   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	   glEnable(GL_COLOR_MATERIAL);
	}
	
	
	
	
	//void Renderer::buildTextures()
//	{
//		
//		GLenum gluerr;
//		GLubyte tex[128][128][4];
//		int x,y,t;
//		int hole_size = 3300; // ~ == 57.45 ^ 2.
//
//		// Generate a texture index, then bind it for future operations.
//		glGenTextures(1, (GLuint*)&(device->texture_id));
//		glBindTexture(GL_TEXTURE_2D,device->texture_id);
//
//		// Iterate across the texture array.
//
//		for(y=0;y<128;y++) {
//		  for(x=0;x<128;x++) {
//
//			 // A simple repeating squares pattern.
//			 // Dark blue on white.
//
//			 if ( ( (x+4)%32 < 8 ) && ( (y+4)%32 < 8)) {
//				tex[x][y][0]=tex[x][y][1]=0; tex[x][y][2]=120;
//			 } else {
//				tex[x][y][0]=tex[x][y][1]=tex[x][y][2]=240;
//			 }
//
//					 // Make a round dot in the texture's alpha-channel.
//
//					 // Calculate distance to center (squared).
//			 t = (x-64)*(x-64) + (y-64)*(y-64) ;
//
//			 if ( t < hole_size) // Don't take square root; compare squared.
//				tex[x][y][3]=255; // The dot itself is opaque.
//			 else if (t < hole_size + 100)
//				tex[x][y][3]=128; // Give our dot an anti-aliased edge.
//			 else
//				tex[x][y][3]=0;   // Outside of the dot, it's transparent.
//
//		  }
//		}
//
//		// The GLU library helps us build MipMaps for our texture.
//
//		if ((gluerr=gluBuild2DMipmaps(GL_TEXTURE_2D, 4, 128, 128, GL_RGBA,
//					 GL_UNSIGNED_BYTE, (void *)tex))) {
//
//		  fprintf(stderr,"GLULib%s\n",gluErrorString(gluerr));
//		  exit(-1);
//		}
//
//		// Some pretty standard settings for wrapping and filtering.
//		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
//		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
//
//		// We start with GL_DECAL mode.
//		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
//	}
	
	
	
	
	void Renderer::resizeScene( Device * d, int new_width, int new_height )
	{
	
		// Let's not core dump, no matter what.
		if (new_height == 0)
		  new_height = 1;

		glViewport(0, 0, new_width, new_height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		float fovy = 26.0f;  // NOTE: It is possible to calulate the exact (most realistic) fovy of a progmam based on the viewer's distance from the screen.  See OpenGL documentation.
							// 26.0f *roughly* coresponds to a 35mm camera 50mm lens,  45.0f *roughly* coresponds to a 24mm wide angle lense.
		
		float aspect = (GLfloat)new_width/(GLfloat)new_height;
		float zNear = 0.1f;
		float zFar = 10000.0f;
		
		d->getCamera()->setPerspective( d, fovy, aspect, zNear, zFar );
		
		glMatrixMode(GL_MODELVIEW);

		d->width  = new_width;
		d->height = new_height;
	}
		
	void Renderer::loadLights( Device * d)
	{
		d->Light_Ambient[0] =  0.1f;
		d->Light_Ambient[1] =  0.1f;
		d->Light_Ambient[2] =  0.1f;
		d->Light_Ambient[3] =  1.0f;
		
		d->Light_Diffuse[0] =  1.2f;
		d->Light_Diffuse[1] =  1.2f;
		d->Light_Diffuse[2] =  1.2f;
		d->Light_Diffuse[3] =  1.0f;
		
		d->Light_Position[0] = 2.0f;
		d->Light_Position[1] = 2.0f;
		d->Light_Position[2] = 0.0f;
		d->Light_Position[3] = 1.0f;
	}
	
	
	void Renderer::loadTextModes( Device * d)
	{
		d->current_text_mode = 0;
		d->text_mode_string[0] = "GL_DECAL";
		d->text_mode_string[1] = "GL_MODULATE";
		d->text_mode_string[2] = "GL_BLEND";
		d->text_mode_string[3] = "GL_REPLACE";
		
		d->text_modes[0] = GL_DECAL;
		d->text_modes[1] = GL_MODULATE;
		d->text_modes[2] = GL_BLEND;
		d->text_modes[3] = GL_REPLACE;
	}
	
	
	void Renderer::lock( Device * d )
	{
		SDL_mutexP( d->renderer_mutex );
	}
	
	void Renderer::unlock( Device * d )
	{
		SDL_mutexV( d->renderer_mutex );
		
	}

	
}



