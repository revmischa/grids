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
	renderer->drawBox();
}


void cbKeyPressed( unsigned char key, int x, int y )
{
	
}

void cbSpecialKeyPressed( int key, int x, int y )
{
	
	
}

void cbResizeScene( int new_width, int new_height )
{
	renderer->resizeScene( new_width, new_height );
}