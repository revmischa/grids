/*
 *
 *  kaleidoscope_2 -- Grids Visualizer
 *
 *  Created by Patrick Tierney on 8/11/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#include <kaleidoscope/kaleidoscope.h>
#include <grids/interface.h>
//#include <JSON/JSON.h>
#include <grids/protocol.h>

Kaleidoscope::Device * main_device = new Kaleidoscope::Device( );

Kaleidoscope::CursorController * main_cursor = new Kaleidoscope::CursorController( main_device );
Kaleidoscope::Renderer * main_renderer = new Kaleidoscope::Renderer( main_device, 640, 480 );
Kaleidoscope::EventController * main_event = new Kaleidoscope::EventController( main_device );
Kaleidoscope::Camera * main_camera = new Kaleidoscope::Camera( main_device );

// Interface doesn't work
Grids::Interface * main_interface;

Kaleidoscope::Room * main_room = new Kaleidoscope::Room( );

//Kaleidoscope::RoomWalls * main_walls = new Kaleidoscope::RoomWalls();

Kaleidoscope::RenderObject * main_table = new Kaleidoscope::RenderObject( );

static SDL_Surface *gScreen;

int main( int argc, char **argv )
{

	main_device->running = 1;        // Flag telling if the program is running

	main_device->setCursorController( main_cursor );
	main_device->setRenderer( main_renderer );
	main_device->setEventController( main_event );
	main_device->setCamera( main_camera );

	//main_interface->addRoom(  );
	
	std::string new_room_id = main_interface->addRoomDebug( main_device );
	
	// ID => vertex
	std::map< std::string, std::vector< float > > a_table;

	std::vector< float > a_vector;

	a_vector.push_back( -1.0f); a_vector.push_back( -1.0f); a_vector.push_back( -1.0f);
	a_vector.push_back( 1.0f); a_vector.push_back( -1.0f); a_vector.push_back( -1.0f);
	a_vector.push_back( 1.0f); a_vector.push_back( -1.0f); a_vector.push_back( 1.0f);
	a_vector.push_back( -1.0f); a_vector.push_back( -1.0f); a_vector.push_back( 1.0f);

	a_vector.push_back( -1.0f); a_vector.push_back( -1.0f); a_vector.push_back( -1.3f);
	a_vector.push_back( -1.0f); a_vector.push_back( 1.0f); a_vector.push_back( -1.3f);
	a_vector.push_back( 1.0f); a_vector.push_back( 1.0f); a_vector.push_back( -1.3f);
	a_vector.push_back( 1.0f); a_vector.push_back( -1.0f); a_vector.push_back( -1.3f);

	main_device->room_objects_hash[ new_room_id ][ "table12345"] = a_vector;

	//main_room->addObject( main_device, main_walls );

	//main_room->addObject( main_device, a_table );

	main_device->x_pos = 200;
	main_device->y_pos = 100;

    // Initialize SDL

	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
	  printf("Unable to init SDL: %s\n", SDL_GetError());
	  return 1;
	}

    if (SDLNet_Init() != 0) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(2);
    }

	
	main_interface = new Grids::Interface( "localhost" );

	int value;

    // Don't set color bit sizes (SDL_GL_RED_SIZE, etc)
    //    Mac OS X will always use 8-8-8-8 ARGB for 32-bit screens and
    //    5-5-5 RGB for 16-bit screens

    // Request a 16-bit depth buffer (without this, there is no depth buffer)
    value = 16;
    SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, value);


    // Request double-buffered OpenGL
    //     The fact that windows are double-buffered on Mac OS X has no effect
    //     on OpenGL double buffering.
    value = 1;
    SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, value);

	gScreen = SDL_SetVideoMode(main_device->width, main_device->height, 0,
		SDL_OPENGL | SDL_HWSURFACE );
	// 0 automatically selects the best availible BPP
	// SDL_HWSURFACE : use hardware rendering

	if ( gScreen == NULL )
	{
	  printf("Unable to create window: %s\n", SDL_GetError());
	  return 1;
	}

	SDL_ShowCursor( SDL_DISABLE );

	main_renderer->prepare( main_device );

	SDL_Event event;

    // Main rendering loop
    do
    {
		main_event->checkEvents( main_device );
		main_camera->doMovementFPS( main_device );
		main_renderer->renderAll( main_device );

        // Swap front and back buffers (we use a double buffered display)
		SDL_GL_SwapBuffers ();

		SDL_PollEvent (&event);

		if( event.type == SDL_QUIT )
		{
			main_device->running = 0;
		}
	}
    while( main_device->running );

    // Cleanup
    SDL_Quit();

    // Exit program
    return 0;
}


