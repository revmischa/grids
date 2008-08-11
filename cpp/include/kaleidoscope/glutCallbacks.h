/*
 *  glutCallbacks.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/9/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


void cbRenderScene( void )
{
	main_device->getRenderer()->renderAll();
}

void cbKeyPressed( unsigned char key, int x, int y )
{
	main_device->getEventController()->keyPressedCall( key, x, y );
}

void cbSpecialKeyPressed( int key, int x, int y )
{
	main_device->getEventController()->specialKeyPressedCall( key, x, y );
}

void cbKeyUp( unsigned char key, int x, int y )
{
	main_device->getEventController()->keyUpCall( key, x, y );
}

void cbSpecialKeyUp( int key, int x, int y )
{
	main_device->getEventController()->specialKeyUpCall( key, x, y );
}

void cbMousePressed( int button, int state, int x, int y )
{
	main_device->getEventController()->mousePressedCall( button, state, x, y );
}

void cbMouseMoved( int x, int y )
{
	main_device->getEventController()->mouseMovedCall( x, y );
}

void cbResizeScene( int new_width, int new_height )
{
	main_device->getRenderer()->resizeScene( new_width, new_height );
}