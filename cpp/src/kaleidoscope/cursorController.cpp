/*
 *  cursorController.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
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

#include <kaleidoscope/cursorController.h>
#include <iostream>
#include <SDL/SDL.h>

namespace Kaleidoscope
{
	CursorController::CursorController( Device * dvc )
	{
	
	}
	

	void CursorController::setPosition( float xScale, float yScale, Device * d )
	{
		SDL_WarpMouse( (int)( xScale * d->width), (int)(  yScale * d->height ) );
	}
	
	void CursorController::setToCenter( Device * d )
	{
		setPosition( 0.5f, 0.5f, d );
	}
	
	Vec2D CursorController::getRelativePosition( Device * d )
	{
		if( d->firstUpdate )
		{
			return Vec2D( 0.5f, 0.5f );
		}
		else
		{
			int x, y;
			SDL_GetMouseState( &x, &y );
			d->mouse_x = x;
			d->mouse_y = y;
							
			return Vec2D( (float)( x ) / (float) d->width, (float)( y ) / (float) d->height );	
		}
	}
	
	void CursorController::lock( Device * d )
	{
		SDL_mutexP( d->cursor_controller_mutex );
	}
	
	void CursorController::unlock( Device * d )
	{
		SDL_mutexV( d->cursor_controller_mutex );
		
	}
	

}

