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

#include <kaleidoscope/renderer.h>

Kaleidoscope::Renderer * renderer = new Kaleidoscope::Renderer(600, 400);

#include <kaleidoscope/render.h> 
#include <kaleidoscope/glutCallbacks.h>
#include <kaleidoscope/glutInit.h>

const char * PROGRAM_TITLE = "Kaleidescope -- Grids Visualizer";
 
int main( int argc, char **argv )
{
	glutInit( &argc, argv);
	
	// To see OpenGL drawing, take out the GLUT_DOUBLE request.
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize( renderer->getWidth(), renderer->getHeight() );
	
	// Open a window 
	renderer->setWindowID( glutCreateWindow( PROGRAM_TITLE ) );

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
	
	// OK, OpenGL's ready to go.  Let's call our own init function.
	renderer->prepare();
	
	glutMainLoop();

	return 1;
}
	
	
	