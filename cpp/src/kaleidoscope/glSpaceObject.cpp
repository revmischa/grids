


#include <kaleidoscope/glSpaceObject.h>



namespace Kaleidoscope
{

	GLSpaceObject::GLSpaceObject( Device* d, Grids::Value* in_val ) : SpaceObject( d, in_val ) {


	}

	void GLSpaceObject::draw( Device* d ) {

	}

	void GLSpaceObject::create( Device* d, Grids::Value* in_val ) {


	}

	void GLSpaceObject::glDraw( Device* d ){

		glPushMatrix();

		Vec3D abs_pos = getPosition();
		Vec3D abs_rot = getRotation();
		Vec3D abs_scl = getScale();
		// Translate
		glTranslatef(	abs_pos.X, 
					abs_pos.Y, 
					abs_pos.Z	);
		
		glRotatef( abs_rot.X, 1.0f,0.0f,0.0f);
		glRotatef( abs_rot.Y, 0.0f,1.0f,0.0f);
		glRotatef( abs_rot.Z, 0.0f,0.0f,1.0f);
			// Scale
		glScalef( abs_scl.X, 
				abs_scl.Y,
				abs_scl.Z );
		
		lock();
		glDrawGeometry(d);
		unlock();		
		
		glPopMatrix();
	}

	void GLSpaceObject::glDrawGeometry( Device* d ){

		// Draw Room Lines
		if( !( attr[ "lines" ] ) == false )
			{
				glBegin( GL_LINES );
				
				for( int g = 0; g < attr[ "lines" ].size(); g++)
					{
						if( !( attr[ "lines" ][ g ][ "color" ] ) == false )
							{
								int color_index = attr[ "lines" ][ g ][ "color" ].asInt();
						
								glColor4f(	attr[ "color" ][  color_index  ][ 0u ].asDouble(), 
											attr[ "color" ][  color_index  ][ 1u ].asDouble(), 
											attr[ "color" ][  color_index  ][ 2u ].asDouble(), 
											attr[ "color" ][  color_index  ][ 3u ].asDouble()   ); 
							}
					
						int vertice_index_1;
						int vertice_index_2;
					
						for( int h = 0; h < attr[ "lines" ][ g ][ "indices"].size(); h++ )
							{
								vertice_index_1 = attr[ "lines" ][ g ][ "indices"][ h ][ 0u ].asInt();
								vertice_index_2 = attr[ "lines" ][ g ][ "indices"][ h ][ 1u ].asInt();
						
								glVertex3f(		attr[ "vertices" ][ vertice_index_1 ][ 0u ].asDouble(), 
												attr[ "vertices" ][ vertice_index_1 ][ 1u ].asDouble(), 
												attr[ "vertices" ][ vertice_index_1 ][ 2u ].asDouble()	);
										
								glVertex3f(		attr[ "vertices" ][ vertice_index_2 ][ 0u ].asDouble(), 
												attr[ "vertices" ][ vertice_index_2 ][ 1u ].asDouble(), 
												attr[ "vertices" ][ vertice_index_2 ][ 2u ].asDouble()	);
							}
					}
				
				glEnd();
			}
			
		// Draw Room Quads
		if( !( attr[ "quads" ] ) == false )
			{
				glBegin( GL_QUADS );
				
				for( int g = 0; g < attr[ "quads" ].size(); g++)
					{
						if( !( attr[ "quads" ][ g ][ "color" ] ) == false )
							{
								int color_index = attr[ "quads" ][ g ][ "color" ].asInt();
						
								glColor4f(	attr[ "color" ][ color_index ][ 0u ].asDouble(), 
											attr[ "color" ][ color_index ][ 1u ].asDouble(), 
											attr[ "color" ][ color_index ][ 2u ].asDouble(), 
											attr[ "color" ][ color_index ][ 3u ].asDouble()	); 
							}
					
						int vertice_index_1;
						int vertice_index_2;
						int vertice_index_3;
						int vertice_index_4;
					
						for( int h = 0; h < attr[ "quads" ][ g ][ "indices" ].size(); h++ )
							{
								vertice_index_1 = attr[ "quads" ][ g ][ "indices" ][ h ][ 0u ].asInt();
								vertice_index_2 = attr[ "quads" ][ g ][ "indices" ][ h ][ 1u ].asInt();
								vertice_index_3 = attr[ "quads" ][ g ][ "indices" ][ h ][ 2u ].asInt();
								vertice_index_4 = attr[ "quads" ][ g ][ "indices" ][ h ][ 3u ].asInt();
						
						
								glVertex3f(	attr[ "vertices" ][ vertice_index_1 ][ 0u ].asDouble(), 
											attr[ "vertices" ][ vertice_index_1 ][ 1u ].asDouble(), 
											attr[ "vertices" ][ vertice_index_1 ][ 2u ].asDouble()	);
									
								glVertex3f(	attr[ "vertices" ][ vertice_index_2 ][ 0u ].asDouble(), 
											attr[ "vertices" ][ vertice_index_2 ][ 1u ].asDouble(), 
											attr[ "vertices" ][ vertice_index_2 ][ 2u ].asDouble()	);
						
								glVertex3f(	attr[ "vertices" ][ vertice_index_3 ][ 0u ].asDouble(), 
											attr[ "vertices" ][ vertice_index_3 ][ 1u ].asDouble(), 
											attr[ "vertices" ][ vertice_index_3 ][ 2u ].asDouble()	);
									
								glVertex3f(	attr[ "vertices" ][ vertice_index_4 ][ 0u ].asDouble(), 
											attr[ "vertices" ][ vertice_index_4 ][ 1u ].asDouble(), 
											attr[ "vertices" ][ vertice_index_4 ][ 2u ].asDouble()	);									
							} // end for h
					} // end for all quads in room
				
				glEnd();
			} // end if [ Quads ] exists

		/////////////
		// Triangles
		////////////
					
					
		if( !( attr[ "triangles" ] ) == false ) // if there are quads
			{
				glBegin(GL_TRIANGLES);
						
				for( int h = 0; h < attr[ "triangles" ].size(); h++ )
					{
						if( !( attr[ "triangles" ][ h ][ "color" ] ) == false )
							{
								int color_index = attr[ "triangles" ][ h ][ "color" ].asInt();
						
								glColor4f(	attr[ "color" ][ color_index ][ 0u ].asDouble(), 
											attr[ "color" ][ color_index ][ 1u ].asDouble(), 
											attr[ "color" ][ color_index ][ 2u ].asDouble(), 
											attr[ "color" ][ color_index ][ 3u ].asDouble()	);
							}
							
						int vertice_index_1;
						int vertice_index_2;
						int vertice_index_3;
							
						for( int k = 0; k < attr[ "triangles" ][ h ][ "indices" ].size(); k++ )
							{
								vertice_index_1 = attr[ "triangles" ][ h ][ "indices" ][ k ][ 0u ].asInt();
								vertice_index_2 = attr[ "triangles" ][ h ][ "indices" ][ k ][ 1u ].asInt();
								vertice_index_3 = attr[ "triangles" ][ h ][ "indices" ][ k ][ 2u ].asInt();
								
								glVertex3f( attr[ "vertices" ][ vertice_index_1 ][ 0u ].asDouble(),
										  attr[ "vertices" ][ vertice_index_1 ][ 1u ].asDouble(),
										  attr[ "vertices" ][ vertice_index_1 ][ 2u ].asDouble()	);
											
								glVertex3f( attr[ "vertices" ][ vertice_index_2 ][ 0u ].asDouble(),
										  attr[ "vertices" ][ vertice_index_2 ][ 1u ].asDouble(),
										  attr[ "vertices" ][ vertice_index_2 ][ 2u ].asDouble()	);
											
								glVertex3f( attr[ "vertices" ][ vertice_index_3 ][ 0u ].asDouble(),
										  attr[ "vertices" ][ vertice_index_3 ][ 1u ].asDouble(),
										  attr[ "vertices" ][ vertice_index_3 ][ 2u ].asDouble()	);
											
							
							
							} // end for each indice in a triangle								
					} // end for h -- for each triangle						
				glEnd();						
			} // end if Triangles exists			

	}



} // end namespace Kaleidoscope
