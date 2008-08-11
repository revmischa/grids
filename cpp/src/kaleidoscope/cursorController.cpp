/*
 *  cursorController.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "cursorController.h"
#include <iostream>


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
	
	void CursorController::setPosition( float xScale, float yScale )
	{
		glutWarpPointer( (int)( xScale * glutGet( GLUT_WINDOW_WIDTH ) )  , (int)( yScale * glutGet( GLUT_WINDOW_HEIGHT ) ));
		
		 //std::cout <<  glutGet( GLUT_WINDOW_WIDTH )  <<  " : " <<  glutGet( GLUT_WINDOW_HEIGHT ) << std::endl;
		 //std::cout <<  glutGet( GLUT_WINDOW_X )  <<  " : " <<  glutGet( GLUT_WINDOW_Y ) << std::endl;
		 //std::cout <<  device->mouseX  <<  " : " <<  device->mouseY << std::endl;
		 
		//glutWarpPointer( 375, 275 );
	}
	
	void CursorController::setToCenter( )
	{
		setPosition( 0.5f, 0.5f );
	}
	
	Vec2D CursorController::getRelativePosition( Device * d )
	{
		//std::cout <<  d->mouseX  <<  " : " <<  d->mouseY << std::endl;
		if( d->firstUpdate )
		{
			return Vec2D( 0.5f, 0.5f );
		}
		else
		{
			return Vec2D( (float)( d->mouseX ) / (float) glutGet( GLUT_WINDOW_WIDTH ) , (float)(  d->mouseY ) / (float) glutGet( GLUT_WINDOW_HEIGHT )   );	
		}
	}
	

}

