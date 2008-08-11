/*
 *  cursorController.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "cursorController.h"


namespace Kaleidoscope
{
	CursorController::CursorController( Device * dvc )
		: device( dvc )
	{
	
	}
	
	void CursorController::setPosition( float xScale, float yScale )
	{
		glutWarpPointer( (int)( xScale * device->getRenderer()->getWidth() ), (int)( yScale * device->getRenderer()->getHeight() ) );
	}
	
	void CursorController::setToCenter( )
	{
		setPosition( 0.5f, 0.5f );
	}
	
	Vec2D CursorController::getRelativePosition()
	{
		return Vec2D( (float)( device->getEventController()->getMouseX() / device->getRenderer()->getWidth() ), (float)( device->getEventController()->getMouseY() / device->getRenderer()->getHeight() ) );	
	}
	

}

