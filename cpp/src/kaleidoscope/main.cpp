/*
 *  
 *  kaleidoscope_2 -- Grids Visualizer
 *
 *  Created by Patrick Tierney on 8/11/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#include "kaleidoscope/kaleidoscope.h"
#include <grids/interface.h>
//#include <JSON/JSON.h>


Kaleidoscope::Device * main_device = new Kaleidoscope::Device( );

Kaleidoscope::CursorController * main_cursor = new Kaleidoscope::CursorController( main_device );
Kaleidoscope::Renderer * main_renderer = new Kaleidoscope::Renderer( main_device, 640, 480 );
Kaleidoscope::EventController * main_event = new Kaleidoscope::EventController( main_device );
Kaleidoscope::Camera * main_camera = new Kaleidoscope::Camera( main_device );

// Interface doesn't work
Grids::Interface * main_interface = new Grids::Interface( "happiland.net" );

Kaleidoscope::Room * main_room = new Kaleidoscope::Room( );

Kaleidoscope::RoomWalls * main_walls = new Kaleidoscope::RoomWalls();

Kaleidoscope::Table * main_table = new Kaleidoscope::Table( );


int main( int argc, char **argv )
{
    
	int    ok;             // Flag telling if the window was opened
    int    running;        // Flag telling if the program is running
	
	main_device->setCursorController( main_cursor );
	main_device->setRenderer( main_renderer );
	main_device->setEventController( main_event );
	main_device->setCamera( main_camera );
		
	main_device->addRoom( main_room );
	
	std::map< Kaleidoscope::RenderObject *, std::vector< float > > a_table;
	
	std::vector< float > a_vector;
	
	a_vector.push_back( -1.0f); a_vector.push_back( -1.0f); a_vector.push_back( -1.0f);
	a_vector.push_back( 1.0f); a_vector.push_back( -1.0f); a_vector.push_back( -1.0f);
	a_vector.push_back( 1.0f); a_vector.push_back( -1.0f); a_vector.push_back( 1.0f);
	a_vector.push_back( -1.0f); a_vector.push_back( -1.0f); a_vector.push_back( 1.0f);
	
	a_vector.push_back( -1.0f); a_vector.push_back( -1.0f); a_vector.push_back( -1.3f);
	a_vector.push_back( -1.0f); a_vector.push_back( 1.0f); a_vector.push_back( -1.3f);
	a_vector.push_back( 1.0f); a_vector.push_back( 1.0f); a_vector.push_back( -1.3f);
	a_vector.push_back( 1.0f); a_vector.push_back( -1.0f); a_vector.push_back( -1.3f);
	
	a_table[ main_table ] = a_vector;
	
	//main_room->addObject( main_device, main_walls );
	
	main_room->addObject( main_device, a_table );
	
	
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
	// Disable the cursor, allows for smooth mouselook
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


