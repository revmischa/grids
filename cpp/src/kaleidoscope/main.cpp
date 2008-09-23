/*
 *
 *  kaleidoscope_2 -- Grids Visualizer
 *
 *	Created by Patrick Tierney on 8/11/08.
 *
 *	 This file is part of Grids/Kaleidoscope.
 *	 
 *	 Grids/Kaleidoscope is free software: you can redistribute it and/or modify
 *	 it under the terms of the GNU General Public License as published by
 *	 the Free Software Foundation, either version 3 of the License, or
 *	 (at your option) any later version.
 *	 
 *	 Grids/Kaleidoscope is distributed in the hope that it will be useful,
 *	 but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	 GNU General Public License for more details.
 *	 
 *	 You should have received a copy of the GNU General Public License
 *	 along with Grids/Kaleidoscope.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 */


#include <kaleidoscope/kaleidoscope.h>
#include <grids/interface.h>
#include <grids/define.h>
#include <grids/protocol.h>

#include <kaleidoscope/voxelSpace.h>
#include <kaleidoscope/simpleCube.h>

#include <stdlib.h>
#include <string>
#include <SDL_ttf/SDL_ttf.h>
#include <SDL_image/SDL_image.h>

#include <sstream>
#include <assert.h>

Kaleidoscope::Device * main_device = new Kaleidoscope::Device( );

Kaleidoscope::CursorController * main_cursor = new Kaleidoscope::CursorController( main_device );
Kaleidoscope::Renderer * main_renderer = new Kaleidoscope::Renderer( main_device, 640, 480 );
Kaleidoscope::EventController * main_event = new Kaleidoscope::EventController( main_device );
Kaleidoscope::Camera * main_camera = new Kaleidoscope::Camera( main_device );

Kaleidoscope::Builder * main_builder = new Kaleidoscope::Builder( );
Kaleidoscope::Gui * main_gui;
Kaleidoscope::Autodesk3dsLoader * main_loader = new Kaleidoscope::Autodesk3dsLoader( );
Kaleidoscope::ThreadController * main_tc = new Kaleidoscope::ThreadController();
Grids::Interface * main_interface;

Grids::Value main_hash;

static SDL_Surface *gScreen;

#define ROOM_SIZE 100.0f


int main( int argc, char **argv )
{
	std::cout << "device  " << main_device << std::endl;
	
	main_device->running = 1;        // Flag telling if the program is running

	main_device->room_width = ROOM_SIZE; // How wide the rooms are

	main_device->setCursorController( main_cursor );
	main_device->setRenderer( main_renderer );
	main_device->setEventController( main_event );
	main_device->setCamera( main_camera );

	main_device->setBuilder( main_builder );
	main_device->setLoader( main_loader );
	main_device->setThreadController(main_tc);


	Grids::GridsID my_room_id = "Room1";

	main_device->world_hash = Grids::Value(  );

	main_builder->placeRoom( main_device, my_room_id );
	main_builder->buildRoom( main_device, my_room_id );
	
	Grids::GridsID temp_box_id;
		
	srand ( time(NULL) );
	

	Grids::GridsID voxel_id = "Voxel1234";
	
//	Kaleidoscope::VoxelSpace * main_voxel = new Kaleidoscope::VoxelSpace( voxel_id , ROOM_SIZE*10, ROOM_SIZE*10, ROOM_SIZE*10, ROOM_SIZE/2, ROOM_SIZE/2, ROOM_SIZE/2  );
	
//	main_builder->placeObject(	main_device, voxel_id, my_room_id,
//							  Kaleidoscope::Vec3D( 0.0f, 0.0f, 0.0f ),
//							  Kaleidoscope::Vec3D( 1.0f, 1.0f, 1.0f ),
//							  Kaleidoscope::Vec3D( 0.0f, 0.0f, 0.0f )	);
		
//	main_device->setVoxel( main_voxel );

	main_device->x_pos = 200;
	main_device->y_pos = 100;

	//main_builder->placeObject( main_device, loaded_id, my_room_id, Kaleidoscope::Vec3D( 0.0f, 0.0f, 0.0f ), Kaleidoscope::Vec3D( 1.0f, 1.0f, 1.0f ), Kaleidoscope::Vec3D( 0.0f, 0.0f, 0.0f ) );
	//main_loader->load3ds( main_device, loaded_id, "torus.3ds", true );

	// Initialize SDL

	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 ) {
		std::cout << "Unable to init SDL: " << SDL_GetError() << std::endl;
		assert( false ); // end the program and tell us where it ended
	}

    if (SDLNet_Init() != 0) {
		std::cout << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        assert( false ); // end the program and tell us where it ended
    }

	main_interface = new Grids::Interface( main_device, "happiland.net" , new Grids::ObjectController(), new Grids::PersonController(), new Grids::MessengerController() );
	main_device->setInterface( main_interface );

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

//	SDL_Surface * temp_surface = SDL_LoadBMP( "change_room.bmp" );
//	SDL_WM_SetIcon( temp_surface, NULL );
	SDL_WM_SetCaption( "Kaleidoscope -- Grids Visualizer", "Kaleidoscope" );
	
	gScreen = SDL_SetVideoMode(main_device->width, main_device->height, 0, SDL_OPENGL | SDL_HWSURFACE );
	// 0 automatically selects the best availible BPP
	// SDL_HWSURFACE : use hardware rendering

	if ( gScreen == NULL ) {
		std::cout << "Unable to create window: " << SDL_GetError() << std::endl;
		assert( false ); // end the prognam and tell us where it ended
	}
	
	main_gui = new Kaleidoscope::Gui( main_device );
	main_device->setGui( main_gui );

	int text_id = main_gui->addText(main_device, Kaleidoscope::Vec2D( -0.95f, 0.95f ), " " );
	main_gui->addText(main_device, Kaleidoscope::Vec3D( 50.0f, 50.0f, 50.0f ), "point < 50, 50, 50 >" );
	main_gui->addText(main_device, Kaleidoscope::Vec3D( -50.0f, 0.0f, -50.0f ), "point < -50, 0, -50 >" );
	
	
	// ** LOAD THE ROOM WITH SOME RANDOMLY PLACED CUBES ** 
	Kaleidoscope::SimpleCube * main_cube = new Kaleidoscope::SimpleCube( );
	float temp_box_color[ 4 ];
	
	for( int i = 0; i < 20; i++ )
	{
		temp_box_color[ 0 ] = (rand() % 10000)/10000.0f;
		temp_box_color[ 1 ] = (rand() % 10000)/10000.0f;
		temp_box_color[ 2 ] = (rand() % 10000)/10000.0f;
		temp_box_color[ 3 ] = 0.35f;
				
		main_cube->requestCreateCube( main_device, my_room_id, 
									 Kaleidoscope::Vec3D(	ROOM_SIZE - (rand() % 10000)/10000.0f * ROOM_SIZE * 2.0f,  
															ROOM_SIZE - (rand() % 10000)/10000.0f * ROOM_SIZE * 2.0f, 
															ROOM_SIZE - (rand() % 10000)/10000.0f * ROOM_SIZE * 2.0f ),
									 2.0f, &temp_box_color[ 0 ]  );
		
	}
	
//	main_tc->updateVoxelThreaded( main_device );
//
//	SDL_Surface * temp_image = IMG_Load( "corona.png" );
//
//  main_builder->packImage(main_device, "temp_image232", temp_image );
//
//	delete temp_image;
//
//	main_device->loaded_image = main_builder->getImage( main_device, "temp_image232" );
	
	
	main_renderer->prepare( main_device );

	main_device->last_clock = SDL_GetTicks();

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
		SDL_GL_SwapBuffers();

	}
    while( main_device->running );

    // Cleanup
    SDL_Quit();
    SDLNet_Quit();

	delete main_device; // Also deletes renderer, cam, interface, loader, etc

    // Exit program
    return 0;
}





