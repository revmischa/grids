/*
 *  main.cpp
 *  kaleidoscope_2
 *
 *  Created by Patrick Tierney on 8/11/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <stdlib.h>    // For malloc() etc.
#include <stdio.h>     // For printf(), fopen() etc.
#include <math.h>      // For sin(), cos() etc.
#include <GL/glfw.h>   // For GLFW, OpenGL and GLU
#include <iostream>
#include <kaleidoscope/kaleidoscope.h>

Kaleidoscope::Device * main_device = new Kaleidoscope::Device( );

Kaleidoscope::CursorController * main_cursor = new Kaleidoscope::CursorController( main_device );
Kaleidoscope::Renderer * main_renderer = new Kaleidoscope::Renderer( main_device, 640, 480 );
Kaleidoscope::EventController * main_event = new Kaleidoscope::EventController( main_device );
Kaleidoscope::Camera * main_camera = new Kaleidoscope::Camera( main_device );



//----------------------------------------------------------------------
// Draw() - Main OpenGL drawing function that is called each frame
//----------------------------------------------------------------------

void Draw( void )
{
    int    width, height;  // Window dimensions
    double t;              // Time (in seconds)
	
	
	
    // Get current time
    t = glfwGetTime();

    // Get window size
    glfwGetWindowSize( &width, &height );

    // Make sure that height is non-zero to avoid division by zero
    height = height < 1 ? 1 : height;

    // Set viewport
    glViewport( 0, 0, width, height );

    // Clear color and depht buffers
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // Set up projection matrix
    glMatrixMode( GL_PROJECTION );    // Select projection matrix
    glLoadIdentity();                 // Start with an identity matrix
    gluPerspective(                   // Set perspective view
        65.0,                         // Field of view = 65 degrees
        (double)width/(double)height, // Window aspect (assumes square pixels)
        1.0,                          // Near Z clipping plane
        100.0                         // Far Z clippling plane
    );

    // Set up modelview matrix
    glMatrixMode( GL_MODELVIEW );     // Select modelview matrix
    glLoadIdentity();                 // Start with an identity matrix
    gluLookAt(                        // Set camera position and orientation
        0.0, 0.0, 10.0,               // Camera position (x,y,z)
        0.0, 0.0, 0.0,                // View point (x,y,z)
        0.0, 1.0, 0.0                 // Up-vector (x,y,z)
    );
	
	      // Let us draw a triangle!
      glBegin( GL_TRIANGLES );          // Tell OpenGL that we want to draw a triangle
      glVertex3f( -5.0f, -4.0f, 0.0f ); // First corner of the triangle
      glVertex3f(  5.0f, -4.0f, 0.0f ); // Second corner of the triangle
      glVertex3f(  0.0f,  4.5f, 0.0f ); // Third corner of the triangle
      glEnd();                          // No more triangles...

    // Here is where actual OpenGL rendering calls would begin...
}


//----------------------------------------------------------------------
// main() - Program entry point
//----------------------------------------------------------------------

int main( int argc, char **argv )
{
    int    ok;             // Flag telling if the window was opened
    int    running;        // Flag telling if the program is running
	
	main_device->setCursorController( main_cursor );
	main_device->setRenderer( main_renderer );
	main_device->setEventController( main_event );
	main_device->setCamera( main_camera );
	
	main_device->x_pos = 200;
	main_device->y_pos = 100; 

    // Initialize GLFW
    glfwInit();

    // Open window
    ok = glfwOpenWindow(
        main_device->width, main_device->height,          // Width and height of window
        8, 8, 8,           // Number of red, green, and blue bits for color buffer
        8,                 // Number of bits for alpha buffer
        24,                // Number of bits for depth buffer (Z-buffer)
        0,                 // Number of bits for stencil buffer
        GLFW_WINDOW        // We want a desktop window (could be GLFW_FULLSCREEN)
    );
	
	glfwSetWindowPos( main_device->x_pos, main_device->y_pos );
	
    // If we could not open a window, exit now
    if( !ok )
    {
        glfwTerminate();
        return 0;
    }

    // Set window title
    glfwSetWindowTitle( "Kaleidoscope -- Grids Visualizer" );

    // Enable sticky keys
    glfwEnable( GLFW_STICKY_KEYS );
	glfwDisable( GLFW_MOUSE_CURSOR ); 
	
	main_renderer->prepare( main_device);
	
    // Main rendering loop
    do
    {
		main_event->checkEvents( main_device );
		main_camera->doMovementFPS( main_device );
		main_renderer->renderAll( main_device );

        // Swap front and back buffers (we use a double buffered display)
        glfwSwapBuffers();

        // Check if the escape key was pressed, or if the window was closed
        running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
    }
    while( running );

    // Terminate GLFW
    glfwTerminate();

    // Exit program
    return 0;
}


