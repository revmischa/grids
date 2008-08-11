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
	main_renderer->setDevice( main_device );
	main_renderer->renderAll();
}

void cbKeyPressed( unsigned char key, int x, int y )
{
	main_event->setDevice( main_device );
	main_event->keyPressedCall( key, x, y );
}

void cbSpecialKeyPressed( int key, int x, int y )
{	
	main_event->setDevice( main_device );
	main_event->specialKeyPressedCall( key, x, y );
}

void cbKeyUp( unsigned char key, int x, int y )
{
	main_event->setDevice( main_device );
	main_event->keyUpCall( key, x, y );
}

void cbSpecialKeyUp( int key, int x, int y )
{
	main_event->setDevice( main_device );
	main_event->specialKeyUpCall( key, x, y );
}

void cbMousePressed( int button, int state, int x, int y )
{
	main_event->setDevice( main_device );
	main_event->mousePressedCall( button, state, x, y );
}


void cbMouseDragged( int x, int y )
{
	main_event->setDevice( main_device );
	main_event->mouseDraggedCall( x, y );
}

void cbMouseMoved( int x, int y )
{
	main_event->setDevice( main_device );
	main_event->mouseMovedCall( x, y );
	
	std::cout << "mouse  " << x << " : " << y << std::endl;
}

void cbResizeScene( int new_width, int new_height )
{
	main_renderer->setDevice( main_device );
	main_renderer->resizeScene( new_width, new_height );
}