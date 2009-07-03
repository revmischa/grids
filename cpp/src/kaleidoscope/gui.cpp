/*
 *  gui.cpp
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 8/21/08.
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

#include <kaleidoscope/gui.h>
#include <kaleidoscope/rect.h>
#include <kaleidoscope/device.h>
#include <kaleidoscope/simpleCube.h>
#include <kaleidoscope/textBox.h>

#include <SDL/SDL_opengl.h>
#include <SDL_ttf/SDL_ttf.h>

#define FONT_SCALE_X 0.0008f
#define FONT_SCALE_Y 0.00107f


namespace Kaleidoscope
{
	Gui::Gui( Device * d, Grids::Value* in_val ) : GuiItem( d, in_val )
	{
		TTF_Init();
		
		Utility::puts( "Initialized TTF" );
		
		d->text_hash[ 1u ] = Grids::Value();
		d->text_hash[ 2u ] = Grids::Value();

		Utility::puts( "Loading fonts" );

		d->screen_font = TTF_OpenFont( "../media/Helvetica LT 25 Ultra Light.ttf", 108 );
		d->space_font = TTF_OpenFont( "../media/Helvetica LT 55 Roman.ttf", 108 );

		Utility::puts( "Loaded fonts" );

		SDL_Color text_color = { 255, 255, 255, 0 };
		d->text_color = text_color;

		glGenTextures( 1, &(d->texture) );
		
		setRoomID( d->getMyRoom() );
		
		d->world_hash[ getID() ][ "position" ][ 0u ] = 0.5f;
		d->world_hash[ getID() ][ "position" ][ 1u ] = 0.25f;
		d->world_hash[ getID() ][ "position" ][ 2u ] = 0.0f;

		d->world_hash[ getID() ][ "rotation" ][ 0u ] = 0.0f;
		d->world_hash[ getID() ][ "rotation" ][ 1u ] = 0.0f;
		d->world_hash[ getID() ][ "rotation" ][ 2u ] = 0.0f;

		d->world_hash[ getID() ][ "scale" ][ 0u ] = 1.0f;
		d->world_hash[ getID() ][ "scale" ][ 1u ] = 1.0f;
		d->world_hash[ getID() ][ "scale" ][ 2u ] = 1.0f;
	}

	void Gui::create( Device* d, Grids::Value* in_val ){

	}
	
	////// DEPRECATED
	void Gui::drawGui( Device * d )
	{
		// But, for fun, let's make the text partially transparent too.
		glColor4f(0.6,1.0,0.6,.75);

		glRasterPos2i(2,2);
		irr::core::rect<int> temp_rect = irr::core::rect<int>( 0, 0, d->width / 8, d->height / 8 );

		drawRect( temp_rect );
		drawRectOutline( temp_rect );

		glColor4f(0.9, 0.2, 0.2, .75);
		glRasterPos2i(6,0);
		// *** PRINT BUFFER
	}
	
	void Gui::draw( Device* d ){
		//drawAllOld( d );
	}

	void Gui::drawAllOld( Device * d )
	{
		prepareSpaceText();
		drawSpaceText( d );
		finishSpaceText();

		prepareText( d );
		drawScreenText( d );
		
		drawAll( d ); // Draw all the GuiItem children

		finishText( d );
	}

	void Gui::drawScreenText( Device * d )
	{
		int num_screen_text = d->screen_texts.size();

		int temp_x;
		int temp_y;
		float pos_x;
		float pos_y;

		SDL_Surface * temp_surface;

		for( int i = 0; i < num_screen_text; i++ )
		{
			temp_surface = d->screen_texts[ i ];

			pos_x = d->text_hash[ 1u ][ i ][ "position" ][ 0u ].asDouble();
			pos_y = d->text_hash[ 1u ][ i ][ "position" ][ 1u ].asDouble();

			temp_x = temp_surface->w;
			temp_y = temp_surface->h;

			glBindTexture( GL_TEXTURE_2D, d->texture );

			glTexImage2D(	GL_TEXTURE_2D, 0, GL_RGBA, temp_surface->w, temp_surface->h, 0, GL_RGBA,
							GL_UNSIGNED_BYTE, temp_surface->pixels );

			glColor4f( 1.0f, 1.0f, 1.0f, 0.5f );

			glPushMatrix();

			glTranslatef( pos_x / FONT_SCALE_X, pos_y / FONT_SCALE_Y, 0.0f );

			glBegin(GL_QUADS); {
				glTexCoord2i(0,0); glVertex2i(0, 0);
				glTexCoord2i(1,0); glVertex2i( temp_x*2, 0);
				glTexCoord2i(1,1); glVertex2i( temp_x*2, -temp_y*2);
				glTexCoord2i(0,1); glVertex2i(0, -temp_y*2);
			} glEnd();

			glPopMatrix();
		} // end for screen texts
	}

	void Gui::drawSpaceText( Device * d )
	// This text floats in 3D space, though always faces the camera.  Billboarded text.
	{
		int num_space_text = d->space_texts.size();

		int temp_x;
		int temp_y;
		float pos_x;
		float pos_y;
		float pos_z;

		Vec3D cam_position = d->getCamera()->getPosition( d );

		float cam_x = cam_position.X;
		float cam_y = cam_position.Y;
		float cam_z = cam_position.Z;

		SDL_Surface * temp_surface;

		for( int i = 0; i < num_space_text; i++ )
		{
			temp_surface = d->space_texts[ i ];

			pos_x = d->text_hash[ 2u ][ i ][ "position" ][ 0u ].asDouble();
			pos_y = d->text_hash[ 2u ][ i ][ "position" ][ 1u ].asDouble();
			pos_z = d->text_hash[ 2u ][ i ][ "position" ][ 2u ].asDouble();

			temp_x = temp_surface->w;
			temp_y = temp_surface->h;

			glBindTexture( GL_TEXTURE_2D, d->texture );

			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, temp_surface->w, temp_surface->h, 0, GL_RGBA,
						 GL_UNSIGNED_BYTE, temp_surface->pixels );

			glColor4f( 1.0f, 1.0f, 1.0f, 0.75f );

			glPushMatrix();

			glTranslatef( pos_x, pos_y, pos_z );

			//std::cout << pos_x << " : " << pos_y << " : " << pos_z << std::endl;
			//std::cout << cam_x << " : " << cam_y << " : " << cam_z << std::endl;

			billboardSphericalBegin(cam_x, cam_y, cam_z, pos_x, pos_y, pos_z );

			float temp_scalar = (cam_x - pos_x) * (cam_x - pos_x)  +
								(cam_y - pos_y) * ( cam_y - pos_y ) +
								( cam_z + pos_z ) * ( cam_z + pos_z )	;

			temp_scalar = sqrt( temp_scalar );

			//std::cout << temp_scalar << std::endl;
			temp_scalar *= 0.0001f;

			//std::cout << temp_scalar << std::endl;

			glScalef(temp_scalar, temp_scalar, temp_scalar );

			glBegin(GL_QUADS); {
				glTexCoord2i(0,0); glVertex2i(-temp_x, temp_y );
				glTexCoord2i(1,0); glVertex2i( temp_x, temp_y );
				glTexCoord2i(1,1); glVertex2i( temp_x, -temp_y );
				glTexCoord2i(0,1); glVertex2i(-temp_x, -temp_y );
			} glEnd();

			billboardEnd();

			glPopMatrix();
		} // end for screen texts
	}

	void Gui::prepareSpaceText( )
	{
		glPushMatrix();

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glEnable( GL_TEXTURE_2D );

		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	}

	void Gui::finishSpaceText()
	{
		glPopMatrix();
	}

	void Gui::billboardSphericalBegin(	float camX, float camY, float camZ,
									float objPosX, float objPosY, float objPosZ)
	// Billboard Spherical Rotates the object in two axis to always face the camera
	{

		float angleCosine;

		// objToCamProj is the vector in world coordinates from the
		// local origin to the camera projected in the XZ plane

		// This is the original lookAt vector for the object
		// in world coordinates

		// normalize both vectors to get the cosine directly afterwards
		Vec3D objToCamProjVec = Vec3D( camX - objPosX, 0.0f, camZ - objPosZ );
		Vec3D lookAtVec = Vec3D( 0.0f, 0.0f, 1.0f );

		objToCamProjVec.normalize();

		// easy fix to determine wether the angle is negative or positive
		// for positive angles upAux will be a vector pointing in the
		// positive y direction, otherwise upAux will point downwards
		// effectively reversing the rotation.

		Vec3D upAuxVec = lookAtVec.crossProduct( objToCamProjVec );

		// compute the angle
		angleCosine = lookAtVec.dotProduct( objToCamProjVec ) ;

		// perform the rotation. The if statement is used for stability reasons
		// if the lookAt and objToCamProj vectors are too close together then
		// |angleCosine| could be bigger than 1 due to lack of precision
		//if ( (angleCosine < 0.9999) && (angleCosine > -0.9999) )
		{
			glRotatef( acos(angleCosine)*180/irr::core::PI, upAuxVec.X, upAuxVec.Y, upAuxVec.Z );
		}

		// so far it is just like the cylindrical billboard. The code for the
		// second rotation comes now
		// The second part tilts the object so that it faces the camera

		// objToCam is the vector in world coordinates from
		// the local origin to the camera

		Vec3D objToCamVec = Vec3D( camX - objPosX, camY - objPosY, camZ - objPosZ );

		// Normalize to get the cosine afterwards

		objToCamVec.normalize();

		// Compute the angle between objToCamProj and objToCam,
		//i.e. compute the required angle for the lookup vector

		angleCosine = objToCamProjVec.dotProduct( objToCamVec ) ;

		// Tilt the object. The test is done to prevent instability
		// when objToCam and objToCamProj have a very small
		// angle between them

		if ( (angleCosine < 0.99990) && (angleCosine > -0.9999) )
		{
			if (objToCamVec.Y < 0)
			{
				glRotatef(acos(angleCosine)*180/irr::core::PI,1,0,0);
			}
			else
			{
				glRotatef(acos(angleCosine)*180/irr::core::PI,-1,0,0);
			}
		}
	}

	void Gui::billboardEnd()
	{
		// restore the previously
		// stored modelview matrix
		//glPopMatrix();
	}




	void Gui::prepareGui( Device * d)
	{
		glLoadIdentity();

		// We need to change the projection matrix for the text rendering.
		glMatrixMode(GL_PROJECTION);

		// But we like our current view too; so we save it here.
		glPushMatrix();

		// Now we set up a new projection for the text.
		glLoadIdentity();
		glOrtho(0, d->width ,0, d->height,-1.0,1.0);

		// Lit or textured text looks awful.
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		// We don't want depth-testing either.
		glDisable(GL_DEPTH_TEST);
	}

	void Gui::prepareText( Device * d )
	{
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glScalef( FONT_SCALE_X, FONT_SCALE_Y , 1.0f);

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glEnable( GL_TEXTURE_2D );
	}

	void Gui::finishText( Device * d )
	{
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	}

	void Gui::finishGui( Device * d)
	{
		// Done with this special projection matrix.  Throw it away.
		glPopMatrix();
	}


	void Gui::guiEvents( Device * d )
	{


	}

	void Gui::registerClick( int mouse_button, int mouse_x, int mouse_y )
	{
		if( mouse_button == SDL_BUTTON_LEFT )
		{
			std::cout << mouse_x << " : " << mouse_y << std::endl;
			
			
		}
	}

	void Gui::drawRect( irr::core::rect< int > in_rect )
	{
		float rect_color[] = { 0.2, 0.2, 0.2, 0.75 };
		drawRect( in_rect, &rect_color[0] );
	}

	void Gui::drawRect( irr::core::rect< int > in_rect, float * color )
	{
		glColor4i( color[0], color[1], color[2], color[3] );

		glBegin(GL_QUADS);
		glVertex3f( in_rect.UpperLeftCorner.X, in_rect.UpperLeftCorner.Y, 0.0f);
		glVertex3f( in_rect.LowerRightCorner.X, in_rect.UpperLeftCorner.Y, 0.0f);
		glVertex3f( in_rect.LowerRightCorner.X, in_rect.LowerRightCorner.Y, 0.0f);
		glVertex3f( in_rect.UpperLeftCorner.X, in_rect.LowerRightCorner.Y, 0.0f);
		glEnd();
	}

	void Gui::drawRectOutline( irr::core::rect< int > in_rect )
	{
		float rect_color[] = { 1.0f, 1.0f, 1.0f, 0.75 };
		drawRectOutline( in_rect, &rect_color[0] );
	}

	void Gui::drawRectOutline( irr::core::rect< int > in_rect, float * color )
	{
		glColor4i( color[0], color[1], color[2], color[3] );

		glBegin(GL_LINES);
		glVertex3f( in_rect.UpperLeftCorner.X, in_rect.UpperLeftCorner.Y, 0.0f);
		glVertex3f( in_rect.LowerRightCorner.X, in_rect.UpperLeftCorner.Y, 0.0f);

		glVertex3f( in_rect.LowerRightCorner.X, in_rect.UpperLeftCorner.Y, 0.0f);
		glVertex3f( in_rect.LowerRightCorner.X, in_rect.LowerRightCorner.Y, 0.0f);

		glVertex3f( in_rect.LowerRightCorner.X, in_rect.LowerRightCorner.Y, 0.0f);
		glVertex3f( in_rect.UpperLeftCorner.X, in_rect.LowerRightCorner.Y, 0.0f);

		glVertex3f( in_rect.UpperLeftCorner.X, in_rect.LowerRightCorner.Y, 0.0f);
		glVertex3f( in_rect.UpperLeftCorner.X, in_rect.UpperLeftCorner.Y, 0.0f);
		glEnd();
	}

	int Gui::addText( Device * d, Vec2D text_position, std::string text_string )
	// Adds text at a given position
	// Position designates how far from the center the text is
	// 0.0, 0.0 is mid screen
	// 1.0, 1.0 is upper right corner
	{
		int new_index = d->screen_texts.size();

		d->text_hash[ 1u ][ new_index ][ "type" ] = 1u; // 1 == 2D text
		d->text_hash[ 1u ][ new_index ][ "position" ][ 0u ] = text_position.X;
		d->text_hash[ 1u ][ new_index ][ "position" ][ 1u ] = text_position.Y;
		d->text_hash[ 1u ][ new_index ][ "text" ] = text_string;

		if( d->DEBUG )
			std::cout << "Attempting to create surface" << std::endl;

		if( d->screen_font == NULL ) {
			std::cerr << "No font loaded, returning" << std::endl;
			return -1;
		}
	
		SDL_Surface * temp_surface = createSDLTextSurface( d, text_string, d->screen_font );

		if( d->DEBUG )
			std::cout << "Created surface" << std::endl;

		d->text_hash[ 1u ][ new_index ][ "x" ] = temp_surface->w;
		d->text_hash[ 1u ][ new_index ][ "y" ] = temp_surface->h;

		d->screen_texts.push_back( temp_surface );
	
		if( d->DEBUG )
			std::cout << "Pushed surface, returing" << std::endl;
		
		return new_index;
	}

	int Gui::addText( Device * d, Vec3D text_position, std::string text_string )
	{
		int new_index = d->space_texts.size();

		d->text_hash[ 2u ][ new_index ][ "type" ] = 2u; // 2 == 3D text
		d->text_hash[ 2u ][ new_index ][ "position" ][ 0u ] = text_position.X;
		d->text_hash[ 2u ][ new_index ][ "position" ][ 1u ] = text_position.Y;
		d->text_hash[ 2u ][ new_index ][ "position" ][ 2u ] = text_position.Z;
		d->text_hash[ 2u ][ new_index ][ "text" ] = text_string;

		SDL_Surface * temp_surface = createSDLTextSurface( d, text_string, d->space_font );

		d->text_hash[ 2u ][ new_index ][ "x" ] = temp_surface->w;
		d->text_hash[ 2u ][ new_index ][ "y" ] = temp_surface->h;

		d->space_texts.push_back( temp_surface );

		return new_index;
	}

	SDL_Surface * Gui::createSDLTextSurface( Device * d, std::string in_string, TTF_Font * in_font )
	{
		if( d->DEBUG > 3)
			std::cout << "Attempting TTF_RenderText_Blended" << std::endl;

		SDL_Surface * new_text_surface_1 = TTF_RenderText_Blended( in_font, in_string.c_str(), d->text_color );

		if( d->DEBUG > 3)
			std::cout << "Succeeded in TTF_RenderTextBlended" << std::endl; 
		
		int x = 1;
		while(x < new_text_surface_1->w)
		{
			x*=2;
		}

		int y = 1;
		while(y < new_text_surface_1->h)
		{
			y*=2;
		}

		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				Uint32 rmask = 0xff000000;
				Uint32 gmask = 0x00ff0000;
				Uint32 bmask = 0x0000ff00;
				Uint32 amask = 0x000000ff;
		#else
				Uint32 rmask = 0x000000ff;
				Uint32 gmask = 0x0000ff00;
				Uint32 bmask = 0x00ff0000;
				Uint32 amask = 0xff000000;
		#endif

		SDL_Surface * new_text_surface_2 = SDL_CreateRGBSurface( SDL_HWSURFACE, x, y, 32, rmask, gmask, bmask, amask);
		// SDL_HWSURFACE -- create the text in video memory
		// SDL_SWSURFACE -- create the text in system memory

		SDL_SetAlpha( new_text_surface_1, 0, 0);

		SDL_BlitSurface(new_text_surface_1, 0, new_text_surface_2, 0);

		return new_text_surface_2;
	}


	std::string Gui::getText( Device * d, int text_type, int text_id )
	{
		return d->text_hash[ text_type ][ text_id ][ "text" ].asString();
	}

	void Gui::updateText( Device * d, int text_type, int text_id, std::string new_text )
	{
		d->text_hash[ text_type ][ text_id ][ "text" ] = new_text;
	}

	void Gui::appendText( Device * d, int text_type, int text_id, std::string new_append )
	{
		if( text_type == 1u )
		{
			std::string new_text = d->text_hash[ 1u ][ text_id ][ "text" ].asString() + new_append;

			d->text_hash[ 1u ][ text_id ][ "text" ] = new_text;

			// update text surface

			SDL_Surface * temp_surface = createSDLTextSurface( d, new_text, d->screen_font );

			d->text_hash[ 1u ][ text_id ][ "x" ] = temp_surface->w;
			d->text_hash[ 1u ][ text_id ][ "y" ] = temp_surface->h;

			delete d->screen_texts[ text_id ];

			d->screen_texts[ text_id ] = temp_surface;

		}
		else if( text_type == 2u )
		{
			d->text_hash[ 2u ][ text_id ][ "text" ] = d->text_hash[ 2u ][ text_id ][ "text" ].asString() + new_append;
		}
	}


	// What does text_type mean
	void Gui::parseText( Device * d, int text_type, int text_id )
	{

		if( text_type == 1u)
		{
			parseString( d,  d->text_hash[ 1u ][ text_id ][ "text" ].asString() );

			d->text_hash[ 1u ][ text_id ][ "text" ] = " ";

			SDL_Surface * temp_surface = createSDLTextSurface( d, " ", d->screen_font );

			d->text_hash[ 1u ][ text_id ][ "x" ] = temp_surface->w;
			d->text_hash[ 1u ][ text_id ][ "y" ] = temp_surface->h;

			delete d->screen_texts[ text_id ];

			d->screen_texts[ text_id ] = temp_surface;
		}
		else if( text_type == 2u )
		{
			parseString( d, d->text_hash[ 2u ][ text_id ][ "text" ].asString() );

			d->text_hash[ 2u ][ text_id ][ "text" ] = " ";
		}

	}

	void Gui::parseString( Device * d, std::string in_string )
	{
		if( ( in_string == " create room" ) ||
			( in_string == " add room" ) ||
			( in_string == " ao" )	) // " ao" for dvorak support
		{
			d->getInterface()->createRoom();
		}
		
		if( (in_string == " obj") || (in_string == " add object" )	) 
		{
			d->getInterface()->addObject( d, d->getMyRoom() );
		}
		
		if( in_string == " add cube" ) 
		{
			
			float room_width = d->getRoomWidth();
			
			float temp_color[ 4u ];
			temp_color[ 0u ] = (rand() % 10000)/10000.0f;
			temp_color[ 1u ] = (rand() % 10000)/10000.0f;
			temp_color[ 2u ] = (rand() % 10000)/10000.0f;
			temp_color[ 3u ] = 0.35f;
			
			// Create a cube giving it the device, the room, the position, the size, and the color
			SimpleCube::requestCreate(d, d->getMyRoom(), Vec3D( 	room_width - (rand() % 10000)/10000.0f * room_width * 2.0f, 
														room_width - (rand() % 10000)/10000.0f * room_width * 2.0f, 
														room_width - (rand() % 10000)/10000.0f * room_width * 2.0f ), 
								2.0f, &temp_color[ 0 ] );
		}
		
		if( in_string == " update cube" )
		{
		   d->getInterface()->getObjectController()->requestUpdatePosition(d, d->temp_box_id, d->getMyRoom(), Vec3D( -50.0f, -50.0f, -50.0f ) );
		   
		}
		
		
		if( in_string == " update" ) 
		{
			d->getThreadController()->updateVoxelThreaded( d );
		}
		
		if( in_string == " ex" ) 
		{
			d->getThreadController()->createRoomDebugThreaded( d );
		}
	}

	GridsID Gui::requestCreateTextBox( Device* d, Vec3D in_pos, Vec3D in_scl, std::string box_text ){
		Utility::puts( "Gui::requestCreateTextBox" );
		return TextBox::requestCreateTextBox( d, getID(), in_pos, in_scl, box_text );		
	}

	void Gui::removeLastCharacter( Device * d, int text_type, int text_id )
	{
		std::string temp_string = d->text_hash[ text_type ][ text_id ][ "text" ].asString();
		int string_length = temp_string.size();

		if( string_length <= 1 )
		{
			d->text_hash[ text_type ][ text_id ][ "text" ] = " ";
		}
		else
		{
			d->text_hash[ text_type ][ text_id ][ "text" ] = temp_string.substr(0, string_length - 1);
		}

		if( text_type == 1u )
		{
			SDL_Surface * temp_surface = createSDLTextSurface( d, d->text_hash[ text_type ][ text_id ][ "text" ].asString(), d->screen_font );

			d->text_hash[ 1u ][ text_id ][ "x" ] = temp_surface->w;
			d->text_hash[ 1u ][ text_id ][ "y" ] = temp_surface->h;

			delete d->screen_texts[ text_id ];

			d->screen_texts[ text_id ] = temp_surface;
		}
	}

	void Gui::clearText( Device * d, int text_type, int text_id )
	{
		d->text_hash[ text_type ][ text_id ][ "text" ] = " ";

		if( text_type == 1u )
		{

			SDL_Surface * temp_surface = createSDLTextSurface( d, d->text_hash[ text_type ][ text_id ][ "text" ].asString(), d->screen_font );

			d->text_hash[ 1u ][ text_id ][ "x" ] = temp_surface->w;
			d->text_hash[ 1u ][ text_id ][ "y" ] = temp_surface->h;

			delete d->screen_texts[ text_id ];

			d->screen_texts[ text_id ] = temp_surface;
		}
	}
	
	void Gui::lock( Device * d )
	{
		SDL_LockMutex( d->gui_mutex );
	}
	
	void Gui::unlock( Device * d )
	{
		SDL_UnlockMutex( d->gui_mutex );
	}
	
	

}
