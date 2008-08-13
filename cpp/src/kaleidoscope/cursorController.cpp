/*
 *  cursorController.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "kaleidoscope/cursorController.h"
#include <iostream>
#include <GL/glfw.h>


namespace Kaleidoscope
{
	CursorController::CursorController( Device * dvc )
		: device( dvc )
	{
	
	}
	
	void CursorController::setDevice( Device * dvc )
	{
		device = dvc;
	}
	
	void CursorController::setPosition( float xScale, float yScale, Device * d )
	{
		glfwSetMousePos( (int)( xScale * d->width), (int)(  yScale * device->height ) ) ;
	}
	
	void CursorController::setToCenter( )
	{
		//setPosition( 0.5f, 0.5f );
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
			glfwGetMousePos( &x, &y );
			d->mouseX = x;
			d->mouseY = y;
							
			return Vec2D( (float)( x ) / (float) d->width, (float)( y ) / (float) d->height );	
		}
	}
	

}

