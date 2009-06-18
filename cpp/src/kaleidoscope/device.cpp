/*
 *  device.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
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
 */

#include <kaleidoscope/device.h>
#include <assert.h>

namespace Kaleidoscope
{

	Device::Device( int screen_width, int screen_height )
	{
		DEBUG = 3;
	  
		createMutexes(); // This must be called first

		loadIDFromFile();

		initOtr(); // This has a wacky setup due to c structures
		
		cursor_controller = new CursorController( this );
		renderer = new Renderer( this, screen_width, screen_height );
		event_controller = new EventController( this );
		cam = new Camera( this );
		builder = new Builder( );
		loader = new Autodesk3dsLoader( );
		thread_controller = new ThreadController( );
	
	
		voxel = NULL;
		gui = NULL;
		interface = NULL;

		world_hash = Grids::Value(  );

		running = 1;        // Flag telling if the program is running

		srand ( time(NULL) );

		gScreen = NULL;
		
		my_room = "NULL";

		otr_user_state = NULL;
		
		screen_font = NULL;
		space_font = NULL;

		initSDL();
		
		
	}

	Device::~Device( )
	{
		std::cout << "Deleting device" << std::endl;

		if( renderer )
			delete renderer;

		if( event_controller )
			delete event_controller;

		if( cam )
			delete cam;

		if( cursor_controller )
			delete cursor_controller;

		if( builder )
			delete builder;

		if( gui )
			delete gui;

		if( loader )
			delete loader;

		if( voxel )
			delete voxel;

		if( thread_controller )
			delete thread_controller;
		
		if( interface )
			delete interface;



		if( device_mutex )
			SDL_DestroyMutex( device_mutex );

		if( cursor_controller_mutex )
			SDL_DestroyMutex( cursor_controller_mutex );

		if( renderer_mutex )
			SDL_DestroyMutex( renderer_mutex );

		if( event_controller_mutex )
			SDL_DestroyMutex( event_controller_mutex );

		if( cam_mutex )
			SDL_DestroyMutex( cam_mutex );

		if( builder_mutex )
			SDL_DestroyMutex( builder_mutex );

		if( loader_mutex )
			SDL_DestroyMutex( loader_mutex );

		if( thread_controller_mutex )
			SDL_DestroyMutex( thread_controller_mutex );

		if( voxel_mutex )
			SDL_DestroyMutex( voxel_mutex );

		if( gui_mutex )
			SDL_DestroyMutex( gui_mutex );

		if( interface_mutex )
			SDL_DestroyMutex( interface_mutex );

		if( world_hash_mutex )
			SDL_DestroyMutex( world_hash_mutex );



	}

	void Device::initOtr()
	{
		Grids::otr.otr_device = this;
		Grids::otr.init();
	}
  
  
	int Device::initSDL()
	{
		// Initialize SDL

		if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 ) {
			std::cout << "Unable to init SDL: " << SDL_GetError() << std::endl;
			assert( false ); // end the program and tell us where it ended
		}

		if (SDLNet_Init() != 0) {
			std::cout << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
			assert( false ); // end the program and tell us where it ended
		}
		

		// This is not part of SDL, but SDL_net needs to be initiated
		
		interface = createInterface( "67.223.239.201" );
		//interface = createInterface( "happiland.net" );

		if( interface == NULL )
		  return -1;
		else {
		  if( DEBUG ) {
		  std::cout << "Connected to server, continuing to init SDL" << std::endl;
		  }
		}


		// Don't set color bit sizes (SDL_GL_RED_SIZE, etc)
		//    Mac OS X will always use 8-8-8-8 ARGB for 32-bit screens and
		//    5-5-5 RGB for 16-bit screens

		// Request a 16-bit depth buffer (without this, there is no depth buffer)
		int value = 16;
		SDL_GL_SetAttribute (SDL_GL_DEPTH_SIZE, value);

		// Request double-buffered OpenGL
		//     The fact that windows are double-buffered on Mac OS X has no effect
		//     on OpenGL double buffering.
		value = 1;
		SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, value);

		//	SDL_Surface * temp_surface = SDL_LoadBMP( "change_room.bmp" );
		//	SDL_WM_SetIcon( temp_surface, NULL );
		SDL_WM_SetCaption( "Kaleidoscope -- Grids Visualizer", "Kaleidoscope" );

		gScreen = SDL_SetVideoMode( width, height, 0, SDL_OPENGL | SDL_HWSURFACE );
		// 0 automatically selects the best availible BPP
		// SDL_HWSURFACE : use hardware rendering

		if ( gScreen == NULL ) {
			std::cout << "Unable to create window: " << SDL_GetError() << std::endl;
			assert( false ); // end the prognam and tell us where it ended
		}

		createGui();

		getRenderer()->prepare( this );

		last_clock = SDL_GetTicks();
        
        return 0;

	}

	void Device::run()
	{
		getEventController()->checkEvents( this );

		if( type == FPS )
		{
			getCamera()->doMovementFPS( this );
		}
		else if( type == MAYA )
		{
			getCamera()->doMovementMaya( this );
		}

		getRenderer()->renderAll( this );

	}

	Gui * Device::createGui( )
	{
		gui = new Gui( this );

		return gui;
	}

	Grids::Interface * Device::createInterface( std::string server_name )
	{
		interface = new Grids::Interface( this, server_name ,
										 new Grids::ObjectController(),
										 new Grids::PersonController(),
										 new Grids::MessageController()	);

		return interface;
	}

	VoxelSpace * Device::createVoxel()
	{
		Grids::GridsID voxel_id = "Voxel1234";
		
		// Voxel (id, node_potential = the distance to create mesh from each node,
		// 		scan area x,y,z,  steps x,y,z
		voxel = new VoxelSpace( voxel_id, 300.0f, room_width*10, room_width*10, room_width*10,
							   room_width/2, room_width/2, room_width/2  );

		getBuilder()->placeObject(	this, voxel_id, getMyRoom(),
								  Kaleidoscope::Vec3D( 0.0f, 0.0f, 0.0f ),
								  Kaleidoscope::Vec3D( 1.0f, 1.0f, 1.0f ),
								  Kaleidoscope::Vec3D( 0.0f, 0.0f, 0.0f )	);

		return voxel;
	}

	Grids::GridsID Device::getMyID()
	{
		return my_id;				
	}

	void Device::setMyRoom( Grids::GridsID my_id )
	{
		lockDevice();
		my_room = my_id ;
		unlockDevice();
	}

	Grids::GridsID Device::getMyRoom( )
	{
		lockDevice();
		Grids::GridsID temp_room = my_room;
		unlockDevice();

		return temp_room;
	}
	
	// getKnownRooms -- lists all the rooms on the server 
	// some rooms may be hidden / private...
	std::vector< Grids::GridsID > Device::getKnownRooms()
	{
		//return interface->getServerRooms( getMyRoom() );

	}

	OtrlUserState 		Device::getOtrUserState() { return otr_user_state; }
	void 			Device::setOtrUserState( OtrlUserState new_state ) { otr_user_state = new_state; }
	OtrlMessageAppOps* 	Device::getOtrOps() { }
	std::string 		Device::getKeyFile() { return keyfile; }
	void 			Device::setKeyFile( std::string new_file ) { keyfile = new_file; }
	std::string 		Device::getFingerprintFile() { return fprfile; }
	void 			Device::setFingerprintFile( std::string new_file ) { fprfile = new_file; }

	Renderer* 		Device::getRenderer() { return renderer; }
	EventController* 	Device::getEventController() { return event_controller; }

	Camera * Device::getCamera()
	{
		return cam;
	}

	CursorController * Device::getCursorController()
	{
		return cursor_controller;
	}

	Builder * Device::getBuilder()
	{
		return builder;
	}

	Gui * Device::getGui( )
	{
		return gui;
	}

	Grids::Interface * Device::getInterface( )
	{
		return interface;
	}

	Autodesk3dsLoader * Device::getLoader()
	{
		return loader;
	}

	VoxelSpace * Device::getVoxel()
	{
		return voxel;
	}

	ThreadController * Device::getThreadController()
	{
		return thread_controller;
	}

	void Device::setRenderer( Renderer * rnd )
	{
		renderer = rnd;
	}

	void Device::setEventController( EventController * evt)
	{
		event_controller = evt;
	}

	void Device::setCamera( Camera * cm)
	{
		cam = cm;
	}

	void Device::setCursorController( CursorController * crs_control)
	{
		cursor_controller = crs_control;
	}

	void Device::setBuilder( Builder * new_builder )
	{
		builder = new_builder;
	}

	void Device::setGui( Gui *  new_gui )
	{
		gui = new_gui;
	}

	void Device::setInterface( Grids::Interface * new_interface )
	{
		interface = new_interface;
	}

	void Device::setLoader( Autodesk3dsLoader * new_loader )
	{
		loader = new_loader;
	}

	void Device::setVoxel( VoxelSpace * new_voxel )
	{
		voxel = new_voxel;
	}

	void Device::setThreadController( ThreadController * new_tc )
	{
		thread_controller = new_tc;
	}


	void Device::addRoom( Room * r )
	{
		//rooms.push_back( r );
	}

	void Device::lockDevice()
	{
		SDL_LockMutex( device_mutex );
	}

	void Device::unlockDevice()
	{
		SDL_UnlockMutex( device_mutex );
	}

	void Device::lockWorldHash()
	{
		//std::cout << "Lock World Hash" << std::endl;
		SDL_LockMutex( world_hash_mutex );
	}

	void Device::unlockWorldHash()
	{
		//std::cout << "Unlock World Hash" << std::endl;
		SDL_UnlockMutex( world_hash_mutex );
	}

	void Device::createMutexes()
	{
		device_mutex = SDL_CreateMutex();
		cursor_controller_mutex = SDL_CreateMutex();
		renderer_mutex = SDL_CreateMutex();
		event_controller_mutex = SDL_CreateMutex();

		cam_mutex = SDL_CreateMutex();
		builder_mutex = SDL_CreateMutex();
		loader_mutex = SDL_CreateMutex();
		thread_controller_mutex = SDL_CreateMutex();

		voxel_mutex = SDL_CreateMutex();
		gui_mutex = SDL_CreateMutex();
		interface_mutex = SDL_CreateMutex();
		world_hash_mutex = SDL_CreateMutex();
	}


	////////////////////////////////////
	// METHODS
	///////////////////////////////////

	void Device::setRoomWidth( float in_width )
	{
		lockDevice();
		room_width = in_width;
		unlockDevice();
	}

	float Device::getRoomWidth( )
	{
		float temp_room_width;

		lockDevice();
		temp_room_width = room_width;
		unlockDevice();

		return temp_room_width;
	}

	void Device::loadIDFromFile()
	{
		std::string temp_id;

		temp_id  = Grids::Utility::readIDFromFile();

		if( temp_id.empty( ) ) {
			std::cout << "******************************************" << std::endl;
			std::cout << "No GridsID found in file, create one first" << std::endl;
			std::cout << "******************************************" << std::endl;
		}

		my_id = temp_id;
	}


}
