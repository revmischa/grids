/*
 *  main.cpp
 *  kaleidoscope -- Grids interface and viewer
 *
 *  Created by Patrick Tierney on 8/9/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
 
#include <iostream>
#include <string>

#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <grids/protocol.h>
#include <grids/vector3d.h>
#include <grids/interface.h> // Contains objects, people, messengers, and their controllers

#include <kaleidoscope/kaleidoscope.h>

Kaleidoscope::Device * main_device = new Kaleidoscope::Device( );

Kaleidoscope::CursorController * main_cursor = new Kaleidoscope::CursorController( main_device );
Kaleidoscope::Renderer * main_renderer = new Kaleidoscope::Renderer( main_device, 600, 400 );
Kaleidoscope::EventController * main_event = new Kaleidoscope::EventController( main_device );
Kaleidoscope::Camera * main_camera = new Kaleidoscope::Camera( main_device );

#include <kaleidoscope/render.h> 
#include <kaleidoscope/glutCallbacks.h>
#include <kaleidoscope/glutInit.h>

const char * PROGRAM_TITLE = "Kaleidescope -- Grids Visualizer";
 
int main( int argc, char **argv )
{
	//Kaleidoscope::createDevice( main_device, 600, 400 );
	
	main_device->setCursorController( main_cursor );
	main_device->setRenderer( main_renderer );
	main_device->setEventController( main_event );
	main_device->setCamera( main_camera );
	
	glutInit( &argc, argv);
	
	// To see OpenGL drawing, take out the GLUT_DOUBLE request.
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize( main_device->getRenderer()->getWidth(), main_device->getRenderer()->getHeight() );
	glutInitWindowPosition( 75, 75);
	
	// Open a window 
	 main_device->getRenderer()->setWindowID( glutCreateWindow( PROGRAM_TITLE ) );

	// Register the callback function to do the drawing. 
	// Public static Foo etc
	// &Grids::Foo::cbRenderScene
	glutDisplayFunc( &cbRenderScene );

	// If there's nothing to do, draw.
	glutIdleFunc( &cbRenderScene);

	// It's a good idea to know when our window's resized.
	glutReshapeFunc( &cbResizeScene);

	// And let's get some keyboard input.
	glutKeyboardFunc(&cbKeyPressed);
	glutSpecialFunc(&cbSpecialKeyPressed);
	
	glutKeyboardUpFunc( &cbKeyUp );
	glutSpecialUpFunc( &cbSpecialKeyUp );
	
	glutMotionFunc ( &cbMouseDragged);
	glutPassiveMotionFunc( &cbMouseMoved );
    glutMouseFunc ( &cbMousePressed);

	// OK, OpenGL's ready to go.  Let's call our own init function.
	 main_device->getRenderer()->prepare();
	
	glutMainLoop();

	return 1;
}
	
	
	