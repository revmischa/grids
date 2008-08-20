/*
 *  renderer.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/9/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
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
		d->Blend_On = false;
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
		//drawBox( d );
		
		prepareRender( d );
		
		//d->world_hash;
		
		
														
		for( int i = 0; i < d->world_hash[ "Num_Rooms" ].asInt(); i++)
		{
			Grids::GridsID temp_room = d->world_hash[ "Rooms" ][ i ].asString();
			
			// Translate
			glTranslatef( d->world_hash[ temp_room ][ "Position" ][ "x" ].asDouble(), d->world_hash[ temp_room ][ "Position" ][ "y" ].asDouble(), d->world_hash[ temp_room ][ "Position" ][ "z" ].asDouble() );
			
			//Rotate the calculated amount.
			glRotatef(  d->world_hash[ temp_room ][ "Rotation" ][ "x" ].asDouble() ,1.0f,0.0f,0.0f);
			glRotatef( d->world_hash[ temp_room ][ "Rotation" ][ "y" ].asDouble() ,0.0f,1.0f,0.0f);
			glRotatef( d->world_hash[ temp_room ][ "Rotation" ][ "z" ].asDouble() ,0.0f,0.0f,1.0f);
			
			// Scale
			glScalef( d->world_hash[ temp_room ][ "Scale" ][ "x" ].asDouble(), d->world_hash[ temp_room ][ "Scale" ][ "y" ].asDouble(), d->world_hash[ temp_room ][ "Scale" ][ "z" ].asDouble() ); 
			
			
			// Draw Room Lines
			if( !( d->world_hash[ temp_room ][ "Lines" ] ) == false )
			{
				glBegin( GL_LINES );
				
				for( int g = 0; g < d->world_hash[ temp_room ][ "Lines" ].size(); g++)
				{
					if( !( d->world_hash[ temp_room][ "Lines" ][ g ][ "Color" ] ) == false )
					{
						int color_index = d->world_hash[ temp_room][ "Lines" ][ g ][ "Color" ].asInt();
						
						glColor4f(	d->world_hash[ temp_room][ "Color" ][  color_index  ][ "r" ].asDouble(), 
									d->world_hash[ temp_room][ "Color" ][  color_index  ][ "g" ].asDouble(), 
									d->world_hash[ temp_room][ "Color" ][  color_index  ][ "b" ].asDouble(), 
									d->world_hash[ temp_room][ "Color" ][  color_index  ][ "a" ].asDouble()   ); 
					}
					
					int vertice_index_1;
					int vertice_index_2;
					
					for( int h = 0; h < d->world_hash[ temp_room ][ "Lines" ][ g ][ "Indices"].size(); h++ )
					{
						vertice_index_1 = d->world_hash[ temp_room ][ "Lines" ][ g ][ "Indices"][ h ][ 0u ].asInt();
						vertice_index_2 = d->world_hash[ temp_room ][ "Lines" ][ g ][ "Indices"][ h ][ 1u ].asInt();
						
						glVertex3f(		d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_1 ][ "x" ].asDouble(), 
										d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_1 ][ "y" ].asDouble(), 
										d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_1 ][ "z" ].asDouble()	);
										
						glVertex3f(		d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_2 ][ "x" ].asDouble(), 
										d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_2 ][ "y" ].asDouble(), 
										d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_2 ][ "z" ].asDouble()	);
					}
				}
				
				glEnd();
			}
			
			// Draw Room Quads
			if( !( d->world_hash[ temp_room ][ "Quads" ] ) == false )
			{
				glBegin( GL_QUADS );
				
				for( int g = 0; g < d->world_hash[ temp_room ][ "Quads" ].size(); g++)
				{
					if( !( d->world_hash[ temp_room][ "Quads" ][ g ][ "Color" ] ) == false )
					{
						int color_index = d->world_hash[ temp_room][ "Quads" ][ g ][ "Color" ].asInt();
						
						glColor4f(	d->world_hash[ temp_room ][ "Color" ][ color_index ][ "r" ].asDouble(), 
									d->world_hash[ temp_room ][ "Color" ][ color_index ][ "g" ].asDouble(), 
									d->world_hash[ temp_room ][ "Color" ][ color_index ][ "b" ].asDouble(), 
									d->world_hash[ temp_room ][ "Color" ][ color_index ][ "a" ].asDouble()	); 
					}
					
					int vertice_index_1;
					int vertice_index_2;
					int vertice_index_3;
					int vertice_index_4;
					
					for( int h = 0; h < d->world_hash[ temp_room ][ "Quads" ][ g ][ "Indices" ].size(); h++ )
					{
						vertice_index_1 = d->world_hash[ temp_room ][ "Quads" ][ g ][ "Indices" ][ h ][ 0u ].asInt();
						vertice_index_2 = d->world_hash[ temp_room ][ "Quads" ][ g ][ "Indices" ][ h ][ 1u ].asInt();
						vertice_index_3 = d->world_hash[ temp_room ][ "Quads" ][ g ][ "Indices" ][ h ][ 2u ].asInt();
						vertice_index_4 = d->world_hash[ temp_room ][ "Quads" ][ g ][ "Indices" ][ h ][ 3u ].asInt();
						
						glVertex3f(	d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_1 ][ "x" ].asDouble(), 
									d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_1 ][ "y" ].asDouble(), 
									d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_1 ][ "z" ].asDouble()	);
									
						glVertex3f(	d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_2 ][ "x" ].asDouble(), 
									d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_2 ][ "y" ].asDouble(), 
									d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_2 ][ "z" ].asDouble()	);
						
						glVertex3f(	d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_3 ][ "x" ].asDouble(), 
									d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_3 ][ "y" ].asDouble(), 
									d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_3 ][ "z" ].asDouble()	);
									
						glVertex3f(	d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_4 ][ "x" ].asDouble(), 
									d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_4 ][ "y" ].asDouble(), 
									d->world_hash[ temp_room ][ "Vertices" ][ vertice_index_4 ][ "z" ].asDouble()	);									
					} // end for h
				} // end for all quads in room
				
				glEnd();
			} // end if [ Quads ] exists
			
			
			// Draw each object in the room, keeping the translation, scale
			
			if( !(d->world_hash[ temp_room ][ "Objects" ]) == false )
			{
				for( int g = 0; g < d->world_hash[ temp_room ][ "Objects" ].size(); g++ )
				{
					GridsID temp_object =  d->world_hash[ temp_room ][ "Objects" ][ g ].asString();
					
					// Translate
					glTranslatef(	d->world_hash[ temp_object ][ "Position" ][ "x" ].asDouble(), 
									d->world_hash[ temp_object ][ "Position" ][ "y" ].asDouble(), 
									d->world_hash[ temp_object ][ "Position" ][ "z" ].asDouble()	);
					
					//Rotate the calculated amount.
					glRotatef(  d->world_hash[ temp_object ][ "Rotation" ][ "x" ].asDouble() ,1.0f,0.0f,0.0f);
					glRotatef( d->world_hash[ temp_object ][ "Rotation" ][ "y" ].asDouble() ,0.0f,1.0f,0.0f);
					glRotatef( d->world_hash[ temp_object ][ "Rotation" ][ "z" ].asDouble() ,0.0f,0.0f,1.0f);
					
					// Scale
					glScalef(	d->world_hash[ temp_object ][ "Scale" ][ "x" ].asDouble(), 
								d->world_hash[ temp_object ][ "Scale" ][ "y" ].asDouble(), 
								d->world_hash[ temp_object ][ "Scale" ][ "z" ].asDouble()	); 
					
					
					if(  !( d->world_hash[ temp_object ][ "Lines" ] ) == false ) // ! returns isNull(),  so if isNull is false
					{
						// Draw Lines
						std::cout << "ERROR" << std::endl;
					} // end if Lines exists
					
					if( !( d->world_hash[ temp_object ][ "Quads" ] ) == false ) // if there are quads
					{
						prepareQuads();
						
						
						
						for( int h = 0; h < d->world_hash[ temp_object ][ "Quads" ].size(); h++ )
						{
							if( !( d->world_hash[ temp_object ][ "Quads" ][ h ][ "Color" ] ) == false )
							{
								int color_index = d->world_hash[ temp_object ][ "Quads" ][ h ][ "Color" ].asInt();
						
								glColor4f(	d->world_hash[ temp_object ][ "Color" ][ color_index ][ "r" ].asDouble(), 
											d->world_hash[ temp_object ][ "Color" ][ color_index ][ "g" ].asDouble(), 
											d->world_hash[ temp_object ][ "Color" ][ color_index ][ "b" ].asDouble(), 
											d->world_hash[ temp_object ][ "Color" ][ color_index ][ "a" ].asDouble()	);
							}
							
							int vertice_index_1;
							int vertice_index_2;
							int vertice_index_3;
							int vertice_index_4;
							
							for( int k = 0; k < d->world_hash[ temp_object ][ "Quads" ][ h ][ "Indices" ].size(); k++ )
							{
								vertice_index_1 = d->world_hash[ temp_object ][ "Quads" ][ h ][ "Indices" ][ k ][ 0u ].asInt();
								vertice_index_2 = d->world_hash[ temp_object ][ "Quads" ][ h ][ "Indices" ][ k ][ 1u ].asInt();
								vertice_index_3 = d->world_hash[ temp_object ][ "Quads" ][ h ][ "Indices" ][ k ][ 2u ].asInt();
								vertice_index_4 = d->world_hash[ temp_object ][ "Quads" ][ h ][ "Indices" ][ k ][ 3u ].asInt();
								
								glVertex3f( d->world_hash[ temp_object ][ "Vertices" ][ vertice_index_1 ][ "x" ].asDouble(),
											d->world_hash[ temp_object ][ "Vertices" ][ vertice_index_1 ][ "y" ].asDouble(),
											d->world_hash[ temp_object ][ "Vertices" ][ vertice_index_1 ][ "z" ].asDouble()	);
											
								glVertex3f( d->world_hash[ temp_object ][ "Vertices" ][ vertice_index_2 ][ "x" ].asDouble(),
											d->world_hash[ temp_object ][ "Vertices" ][ vertice_index_2 ][ "y" ].asDouble(),
											d->world_hash[ temp_object ][ "Vertices" ][ vertice_index_2 ][ "z" ].asDouble()	);
											
								glVertex3f( d->world_hash[ temp_object ][ "Vertices" ][ vertice_index_3 ][ "x" ].asDouble(),
											d->world_hash[ temp_object ][ "Vertices" ][ vertice_index_3 ][ "y" ].asDouble(),
											d->world_hash[ temp_object ][ "Vertices" ][ vertice_index_3 ][ "z" ].asDouble()	);
											
								glVertex3f( d->world_hash[ temp_object ][ "Vertices" ][ vertice_index_4 ][ "x" ].asDouble(),
											d->world_hash[ temp_object ][ "Vertices" ][ vertice_index_4 ][ "y" ].asDouble(),
											d->world_hash[ temp_object ][ "Vertices" ][ vertice_index_4 ][ "z" ].asDouble()	);
							
							
							} // end for each indice in a quad
								
						} // end for h -- for each quad
						
						finishQuads();
						
					} // end if Quads exists
					
					
				} // end for g
			} // end if Objects
		} // end for i -- per room loop
		
		
		//prepareQuads();
//
//		glColor4f(0.9,0.2,0.2,.75); // set the color
//		
//		std::vector< std::string > temp_rooms = d->rooms; // Check out vector from device
//		
//		std::map< std::string, std::map< std::string, std::vector< float > > > room_objects_hash = d->room_objects_hash;
//						
//		int num_rooms = temp_rooms.size();
//		int num_objects = 0;
//		
//		std::map< std::string, std::vector< float > > object_vertex_hash;
//		std::map< std::string, std::vector< float > >::iterator object_iterator;
//		
//		for( int i = 0; i < num_rooms; i++ ) // Iterate through every stored room
//		{
//			glLoadIdentity(); // Reset matrix. No scaling, rotation, or translation
//			
//			std::string room_id = temp_rooms.at( i ); // Get one room
//						
//			object_vertex_hash = room_objects_hash[ room_id ]; // get a map: Object => vetices of all objects in room
//		
//			 // start an iterator to go through the hash
//			 
//			for( object_iterator = object_vertex_hash.begin(); object_iterator != object_vertex_hash.end(); object_iterator++ ) // Iterate though each object in the room
//			{
//				std::vector< float > temp_vector = object_iterator->second;
//				int num_vertices = temp_vector.size();
//				
//				for( int h = 0; h < num_vertices; h += 3  )
//				{
//					glVertex3f( temp_vector.at( h ), temp_vector.at( h + 1 ), temp_vector.at( h + 2 ) );
//				}
//			}
//			
//		}
//		
//		finishQuads();
//		
//		int num_lines = 100;
		
		//glBegin(GL_LINES);
//		//glHint( GL_LINE_SMOOTH_HINT, GL_NICEST);
//		for(int i=-num_lines;i<=num_lines;++i) {
//			glVertex3f(i,0,-num_lines);
//			glVertex3f(i,0,num_lines);
//
//			glVertex3f(num_lines,0,i);
//			glVertex3f(-num_lines,0,i);
//		}
//		glEnd();
		
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
	
	void Renderer::prepareQuads()
	{
		glBegin(GL_QUADS); 
	}
	
	void Renderer::finishQuads()
	{
		glEnd();
	}
	
	void Renderer::finishRender()
	{
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
	   
	   gluPerspective(45.0f,(GLfloat)new_width/(GLfloat)new_height,0.1f,10000.0f);

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

	
}


