/*
 *  gui.cpp
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 8/21/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <kaleidoscope/gui.h>
#include <SDL/SDL_opengl.h>
#include <kaleidoscope/rect.h>


namespace Kaleidoscope
{
	
	Gui::Gui()
	{
	
	}
	
	void Gui::drawGui( Device * d )
	{
		// But, for fun, let's make the text partially transparent too.
		glColor4f(0.6,1.0,0.6,.75);
				
		// Load a string into a buffer, and then print the buffer
		
		glRasterPos2i(2,2); 
		// *** PRINT BUFFER
		
		// Now we want to render the calulated FPS at the top.
   
		// To ease, simply translate up.  Note we're working in screen
		// pixels in this projection.

		//glTranslatef(0.0f, d->height , 0.0f);

		// Make sure we can read the FPS section by first placing a 
		// dark, mostly opaque backdrop rectangle.
		irr::core::rect<int> temp_rect = irr::core::rect<int>( 0, 0, d->width / 8, d->height / 8 );
		
		drawRect( temp_rect );
		drawRectOutline( temp_rect );

		glColor4f(0.9, 0.2, 0.2, .75);
		glRasterPos2i(6,0);
		// *** PRINT BUFFER
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
	
	

}
