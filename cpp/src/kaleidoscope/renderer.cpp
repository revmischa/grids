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

#include <grids/define.h>

namespace Kaleidoscope
{
	Renderer::Renderer( Device * d, int window_width, int window_height, Grids::Value * in_val ) : Object( d, in_val ) 
	{		

	}
	
	Renderer::~Renderer(){ 

	}
	
	void Renderer::setRenderVars( Device* d, int sw, int sh ){
		d->width = sw;
		d->height = sh;
		
		d->Texture_On = false;
		d->Light_On = false;
		d->Alpha_Add = false;
		d->Blend_On = true;
		d->Filtering_On = false;
		d->Smooth_On = true;
		
		loadLights(d);
		loadTextModes(d);		
	}
	
	int Renderer::getWidth( Device * d )
	{
		int temp_width;
		
		lock( d );
		temp_width = d->width;
		unlock( d );
		
		return temp_width;
	}
	
	int Renderer::getHeight( Device * d )
	{
		int temp_height;
		
		lock( d );
		temp_height = d->height;
		unlock( d );
		
		return d->height;
	}
	
	void Renderer::setWidth( Device * d, int in_width )
	{
		lock( d );
		d->width = in_width;
		unlock( d );
	}
	
	void Renderer::setHeight( Device * d, int in_height )
	{
		d->height = in_height;
	}

	void Renderer::draw( Device* d ){

	}

	void Renderer::create( Device* d, Grids::Value* ){


	}
	
	void Renderer::renderAll( Device * d)
	{		
		prepareRender( d );
				
		//renderWorldHash( d );	
		
		drawAll(d);
		
		
		renderGui( d );
				
		finishRender();
	}
	
	void Renderer::prepareRender( Device * d)
	{
		bool text_on;
		bool light_on;
		bool al_on;
		bool blend_on;
		bool filt_on;
		bool smooth_on;
		
		lock( d );
		text_on = d->Texture_On;
		light_on = d->Light_On;
		al_on = d->Alpha_Add;
		blend_on = d->Blend_On;
		filt_on = d->Filtering_On;
		smooth_on = d->Smooth_On;
		unlock( d );
		
		
		if ( text_on )
		  glEnable(GL_TEXTURE_2D);
	   else
		  glDisable(GL_TEXTURE_2D);

	   if ( light_on ) 
		  glEnable(GL_LIGHTING);
	   else 
		  glDisable(GL_LIGHTING);

		if ( al_on )
		   glBlendFunc(GL_SRC_ALPHA,GL_ONE); 
		else
		   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

		// If we're blending, we don't want z-buffering.
		if ( blend_on )
		   glDisable(GL_DEPTH_TEST); 
		else
		   glEnable(GL_DEPTH_TEST); 

		if ( filt_on ) {
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
											   GL_LINEAR_MIPMAP_LINEAR);
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		} else {
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
											   GL_NEAREST_MIPMAP_NEAREST);
		   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		}
		
		if ( smooth_on )
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
		d->getGui()->drawAllOld( d );
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
		
		lock( d );

	   // Load up the correct perspective matrix; using a callback directly.
	   resizeScene(d, d->width , d->height);

	   // Set up a light, turn it on.
	   glLightfv(GL_LIGHT1, GL_POSITION, d->Light_Position);
	   glLightfv(GL_LIGHT1, GL_AMBIENT,  d->Light_Ambient);
	   glLightfv(GL_LIGHT1, GL_DIFFUSE,  d->Light_Diffuse); 
	   glEnable (GL_LIGHT1); 
		
		unlock( d );
	
	   // A handy trick -- have surface material mirror the color.
	   glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
	   glEnable(GL_COLOR_MATERIAL);
	}
	
	

	
	
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
		
		lock( d );
		
		d->getCamera()->setPerspective( d, fovy, aspect, zNear, zFar );
		
		glMatrixMode(GL_MODELVIEW);

		d->width  = new_width;
		d->height = new_height;
		
		unlock( d );
	}
		
	void Renderer::loadLights( Device * d)
	{
		//lock( d );
		
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
		
		//unlock( d );
	}
	
	
	void Renderer::loadTextModes( Device * d)
	{
		//lock( d );
		
		d->current_text_mode = 0;
		d->text_mode_string[0] = "GL_DECAL";
		d->text_mode_string[1] = "GL_MODULATE";
		d->text_mode_string[2] = "GL_BLEND";
		d->text_mode_string[3] = "GL_REPLACE";
		
		d->text_modes[0] = GL_DECAL;
		d->text_modes[1] = GL_MODULATE;
		d->text_modes[2] = GL_BLEND;
		d->text_modes[3] = GL_REPLACE;
		
		//unlock( d );
	}
	
	
	void Renderer::lock( Device * d )
	{
		SDL_LockMutex( d->renderer_mutex );
	}
	
	void Renderer::unlock( Device * d )
	{
		SDL_UnlockMutex( d->renderer_mutex );
		
	}

	
}



