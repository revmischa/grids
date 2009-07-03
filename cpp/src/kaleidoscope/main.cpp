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

// Some thoughts:
// I. The builder / objects that request to be created will have to embed within themselves a tagging ID, so that whet the GridsID is returned, Kaleidoscope knows which objects was just created.  In other words, there needs to be some emulation of the ability to do 
// my_cube = new SimpleCube(), as things are created with requests to the gridsServer.  
// Opions:
// 1. is_loaded variable, so my_cube->is_loaded is false until the request is satisfied.
// 2. Callbacks -- (will this interfere with threading?)
//
// II. Should Kaleidoscope object classes even exist?  Selection, physics, etc seems appropriately handeled in GridsObject.  Should Kaleidoscope classes simply exist as "built" / loaded objects.  IE Create an object, then build up the appropriate geometry.
// Options
// 1. Visual representation should not have its own class, that is simply loaded
// 2. Inherently different devices should have their own class
//    a, Dumb, static objects
// 	 b, Utility, gets information about the server, etc
//    c, other 
//
// III. There should be a Kaleidoscope::Math namespace, stuff like DistanceToSphereAlongRay should not be in Grids::object.....

#include <kaleidoscope/kaleidoscope.h>

#define ROOM_SIZE 100.0f


int main( int argc, char **argv )
{
	// Specify the width and heigh of the window
	Kaleidoscope::Device * main_device = NULL;
	main_device = new Kaleidoscope::Device( 640, 480 );

	if( main_device == NULL ) {
		std::cerr << "Could not create device" << std::endl;
		return -1;
	}
		
	//Grids::OTR * temp_otr = new Grids::OTR( );

	std::cout << "******  MY ID:  " << main_device->getMyID() << std::endl;

	if( main_device->DEBUG )
		std::cout << "Created device" << std::endl;

	main_device->setRoomWidth( ROOM_SIZE ); // How wide the rooms are, I as of yet dont know the exact "scale"
	// of grids
	
	if( main_device->DEBUG )
		std::cout << "Set room width" << std::endl;
		
	//Sets your room.  This should idealy be through grids
	//main_device->setMyRoom( "62688B92-D3AA-11DD-8EC7-91669DFFFA79" );

	//main_device->getBuilder()->placeRoom( main_device, main_device->getMyRoom() );ccccc

	//main_device->getBuilder()->buildRoom( main_device, main_device->getMyRoom() );
	
	// This is the proper way to create a room, through grids, though at the moment I cannot tell the ID of the room created
	if( main_device->DEBUG )
		std::cout << "Creating Room" << std::endl;
	
	main_device->getInterface()->createRoom();
	
	std::cout << "Creating room";
	
	while( main_device->getMyRoom().compare("NULL") == 0 ){
		sleep(1);
		std::cout << "."; 
	}
	sleep( 2 );
	
	Kal::Utility::puts( "Created room:  " );
	Kal::Utility::puts( main_device->getMyRoom() );
			
	//Kal::Utility::puts( "Creating guitext" );
	
	//int text_id = main_device->getGui()->addText(main_device, Kaleidoscope::Vec2D( -0.95f, 0.95f ), " " );
	//main_device->getGui()->addText(main_device, Kaleidoscope::Vec3D( 50.0f, 50.0f, 50.0f ), "point < 50, 50, 50 >" );
	//main_device->getGui()->addText(main_device, Kaleidoscope::Vec3D( -50.0f, 0.0f, -50.0f ), "point < -50, 0, -50 >" );
	
	Grids::GridsID box_id = main_device->getGui()->requestCreateTextBox( main_device, Kal::Vec3D( -0.95f, 0.0f, 0.0f ), Kal::Vec3D( 1.0f, 1.0f, 0.0f ), "lol dongs" );

	Kal::Utility::puts( "** Created Text Box **" );
	Kal::Utility::puts( box_id );
		
	//Kal::Utility::puts( "Created guitext" ); 


	//Kal::Utility::puts( "Creating boxes");
	//main_device->getBuilder()->createRandomBoxes( main_device, main_device->getMyRoom(), 5 );
	
	//main_device->createVoxel();
	//main_device->getThreadController()->updateVoxelThreaded( main_device );
	
	//Kal::Utility::puts( "Created Voxel" );

	//Kaleidoscope::Utility * main_utility = new Kaleidoscope::Utility();

	//main_utility->displayRooms( main_device, main_device->getMyRoom() );


	//////////////////////////////////
	// IMAGES AND MODEL LOADING STILL BUGGY!
	/////////////////////////////////////
	/*
	Kal::Utility::puts( "Attempting to load image");
	SDL_Surface * temp_image = IMG_Load( "../media/mario.png" );
	Kal::Utility::puts( "Loaded image, packing image.");

	main_device->getBuilder()->packImage(main_device, "Temp_id", temp_image );
	Kal::Utility::puts( "Packed image" );

	delete temp_image;
	*/

	//main_device->getBuilder()->placeObject( main_device, "torus123", 
	//main_device->getMyRoom(), 
	//					  		Kaleidoscope::Vec3D( 0.0f, 0.0f, 0.0f ), 
	//					  		Kaleidoscope::Vec3D( 1.0f, 1.0f, 1.0f ), 
	//					  		Kaleidoscope::Vec3D( 0.0f, 0.0f, 0.0f )	);
	//main_device->getLoader()->load3ds( main_device, "dongs", "../media/torus.3ds", true );
	
	// Main rendering loop
	do{
		main_device->run();

		// Swap front and back buffers (we use a double buffered display)
		SDL_GL_SwapBuffers();
	}
	while( main_device->running );
	
	//SDL_FreeSurface( temp_image);
	
	// Cleanup	
	delete main_device; // Also deletes renderer, cam, interface, loader, etc
	
	TTF_Quit();
	SDLNet_Quit();
	SDL_Quit();
	
	return 0;
}





