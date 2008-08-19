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
#include <grids/define.h>
#include <JSON/JSON.h>
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

Grids::complex_type main_hash;

static SDL_Surface *gScreen;


//*************
//	Notes:  -- Room Is a UUID
//			-- Color is optional
//
//
//	[Room][ID]	[Position]
//				[Scale]
//				[Rotation]
//				[Quads] [ Num Quads ]
//						[1] [Color]
//						[1] [ Vert1 ]
//						[1] [ Vert2 ]
//						[1] [...]
//						[2] [Color]
//						[2] [ Vert1 ]
//						[2] [ Vert2 ]
//						[2] [...]
//						[...]
//				[ Lines ] [Num Lines ]
//						[1] [Color]
//			[Id] [Position]
//				[Scale]
//				[Rotation]
//				[...]
//			[...]
//	[Room][ID] [ ...]
//		[ ... ]
//						
//
//
//**************

int main( int argc, char **argv )
{
	
	main_device->running = 1;        // Flag telling if the program is running
	
	main_hash = Grids::complex_type(  );

	main_device->setCursorController( main_cursor );
	main_device->setRenderer( main_renderer );
	main_device->setEventController( main_event );
	main_device->setCamera( main_camera );

	//main_interface->addRoom(  );
	
	Grids::complex_type temp_hash = Grids::complex_type();
	
	Grids::GridsID room_id = "Room123";
	Grids::GridsID object_id_1 = "Object123";
	
	main_hash[ "Num_Rooms" ] = 1;
	
	main_hash[ "Rooms" ] = Grids::complex_type();
	main_hash[ "Rooms" ][ 0u ] = room_id;
	
	main_hash[ room_id ][ "Position" ] = Grids::complex_type();
	main_hash[ room_id ][ "Position" ][ "x" ] = 0.0f;
	main_hash[ room_id ][ "Position" ][ "y" ] = 0.0f;
	main_hash[ room_id ][ "Position" ][ "z" ] = 0.0f;
	
	main_hash[ room_id ][ "Scale" ] = Grids::complex_type();
	main_hash[ room_id ][ "Scale" ][ "x" ] = 1.0f;
	main_hash[ room_id ][ "Scale" ][ "y" ] = 1.0f;
	main_hash[ room_id ][ "Scale" ][ "z" ] = 1.0f;
	
	main_hash[ room_id ][ "Rotation" ] = Grids::complex_type();
	main_hash[ room_id ][ "Rotation" ][ "x" ] = 0.0f;
	main_hash[ room_id ][ "Rotation" ][ "y" ] = 0.0f;
	main_hash[ room_id ][ "Rotation" ][ "z" ] = 0.0f;
	
	main_hash[ room_id ][ "Quads" ] = Grids::complex_type();
	
	main_hash[ room_id ][ "Lines" ] = Grids::complex_type();
	main_hash[ room_id ][ "Num_Objects" ] = 1;
	main_hash[ room_id ][ "Objects" ] = Grids::complex_type();
	main_hash[ room_id ][ "Objects" ][ 0u ] = object_id_1;
	
	int num_lines = 100;
	
	main_hash[ room_id ][ "Lines" ][ 0u ] = Grids::complex_type();
	
	main_hash[ room_id ][ "Lines" ][ 0u ][ "Color" ] = Grids::complex_type();
	main_hash[ room_id ][ "Lines" ][ 0u ][ "Color" ][ "r" ] = 0.0f;
	main_hash[ room_id ][ "Lines" ][ 0u ][ "Color" ][ "g" ] = 1.0f;
	main_hash[ room_id ][ "Lines" ][ 0u ][ "Color" ][ "b" ] = 0.0f;
	main_hash[ room_id ][ "Lines" ][ 0u ][ "Color" ][ "a" ] = 1.0f;
	
	main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ] = Grids::complex_type();
	
	for(int i = 0; i <= num_lines * 8; i += 4 ) // i * 2 from -num_lines to num_lines --  + 4 Vertices 
	{
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i ] = Grids::complex_type();
		
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i ][ "x" ] = (float)i;
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i ][ "y" ] = 0.0f;
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i ][ "z" ] = (float)-num_lines;
		
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i+1 ] = Grids::complex_type();
		
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i+1 ][ "x" ] = (float)i;
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i+1 ][ "y" ] = 0.0f;
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i+1 ][ "z" ] = (float)num_lines;
		
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i+2 ] = Grids::complex_type();
		
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i+2 ][ "x" ] = (float)num_lines;
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i+2 ][ "y" ] = 0.0f;
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i+2 ][ "z" ] = (float)i;
		
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i+3 ] = Grids::complex_type();
		
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i+3 ][ "x" ] = (float)-num_lines;
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i+3 ][ "y" ] = 0.0f;
		main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ i+3 ][ "z" ] = (float)i;
	}
	
	//std::cout << main_hash[ room_id ][ "Lines" ][ 0u ][ "Vertices" ][ 33 ][ "z" ] << std::endl;
	
	main_hash[ object_id_1 ][ "Room" ] = room_id;
	
	main_hash[ object_id_1 ][ "Position" ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Position" ][ "x" ] = -10.0f;
	main_hash[ object_id_1 ][ "Position" ][ "y" ] = -5.0f;
	main_hash[ object_id_1 ][ "Position" ][ "z" ] = -6.0f;
	
	main_hash[ object_id_1 ][ "Scale" ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Scale" ][ "x" ] = 1.0f;
	main_hash[ object_id_1 ][ "Scale" ][ "y" ] = 1.0f;
	main_hash[ object_id_1 ][ "Scale" ][ "z" ] = 1.0f;
	
	main_hash[ object_id_1 ][ "Rotation" ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Rotation" ][ "x" ] = 0.0f;
	main_hash[ object_id_1 ][ "Rotation" ][ "y" ] = 0.0f;
	main_hash[ object_id_1 ][ "Rotation" ][ "z" ] = 0.0f;
	
	main_hash[ object_id_1 ][ "Quads" ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Quads" ][ 0u ] = Grids::complex_type();
	 
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Color" ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Color" ][ "r" ] = 0.9f;
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Color" ][ "g" ] = 0.2f;
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Color" ][ "b" ] = 0.2f;
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Color" ][ "a" ] = 0.75f;
	
	// Quads have 4 vertices, so 4 more hashes are needed
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ] = Grids::complex_type();
	
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 0u ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 0u ][ "x" ] = -1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 0u ][ "y" ] = -1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 0u ][ "z" ] = -1.0f;
	
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 1u ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 1u ][ "x" ] = 1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 1u ][ "y" ] = -1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 1u ][ "z" ] = -1.0f;
	
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 2u ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 2u ][ "x" ] = 1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 2u ][ "y" ] = -1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 2u ][ "z" ] = 1.0f;
	
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 3u ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 3u ][ "x" ] = -1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 3u ][ "y" ] = -1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 0u ][ "Vertices" ][ 3u ][ "z" ] = 1.0f;
	
	main_hash[ object_id_1 ][ "Quads" ][ 1u ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ] = Grids::complex_type();

	// Quads have 4 vertices, so 4 more hashes are needed
	
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 0u ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 0u ][ "x" ] = -1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 0u ][ "y" ] = -1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 0u ][ "z" ] = -1.3f;
	
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 1u ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 1u ][ "x" ] = -1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 1u ][ "y" ] = 1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 1u ][ "z" ] = -1.3f;
	
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 2u ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 2u ][ "x" ] = 1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 2u ][ "y" ] = 1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 2u ][ "z" ] = -1.3f;
	
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 3u ] = Grids::complex_type();
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 3u ][ "x" ] = 1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 3u ][ "y" ] = -1.0f;
	main_hash[ object_id_1 ][ "Quads" ][ 1u ][ "Vertices" ][ 3u ][ "z" ] = -1.3f;
	
	//main_hash[ object_id_1 ][ "Lines" ] = Grids::complex_type();
	
	main_device->world_hash = main_hash;
	
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


	//main_interface = new Grids::Interface( "happiland.net" );
	//main_device->interface = main_interface;

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

	//SDL_ShowCursor( SDL_DISABLE );

	main_renderer->prepare( main_device );
	
	main_device->last_clock = clock();

	SDL_Event event;

    // Main rendering loop
    do
    {
		main_event->checkEvents( main_device );
		
		if( main_device->type == Kaleidoscope::FPS )
		{
			main_camera->doMovementFPS( main_device );
		}
		else if( main_device->type == Kaleidoscope::MAYA )
		{
			main_camera->doMovementMaya( main_device );
		}
		
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
    SDLNet_Quit();

    // Exit program
    return 0;
}


