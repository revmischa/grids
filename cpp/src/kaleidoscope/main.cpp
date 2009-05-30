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
	
	if( main_device->DEBUG > 1 ){
		std::cout << "Created room:  " << main_device->getMyRoom() << std::endl;
	}
	
	
	if( main_device->DEBUG )
	  std::cout << "Creating guitext" << std::endl;
	
	int text_id = main_device->getGui()->addText(main_device, Kaleidoscope::Vec2D( -0.95f, 0.95f ), " " );
	main_device->getGui()->addText(main_device, Kaleidoscope::Vec3D( 50.0f, 50.0f, 50.0f ), "point < 50, 50, 50 >" );
	main_device->getGui()->addText(main_device, Kaleidoscope::Vec3D( -50.0f, 0.0f, -50.0f ), "point < -50, 0, -50 >" );
	
	if( main_device->DEBUG > 2 ){
		std::cout << "Creating boxes" << std::endl;
	}

	main_device->getBuilder()->createRandomBoxes( main_device, main_device->getMyRoom(), 5 );

	if( main_device->DEBUG )
	  std::cout << "Created guitext" << std::endl;	
	
	main_device->createVoxel();
	main_device->getThreadController()->updateVoxelThreaded( main_device );

	if( main_device->DEBUG )
	  std::cout << "Created Voxel" << std::endl;

	Kaleidoscope::Utility * main_utility = new Kaleidoscope::Utility();

	main_utility->displayRooms( main_device, main_device->getMyRoom() );



	//////////////////////////////////
	// IMAGES AND MODEL LOADING STILL BUGGY!
	/////////////////////////////////////

	SDL_Surface * temp_image = IMG_Load( "../../media/mario.png" );

	main_builder->packImage(main_device, "temp_image232", temp_image );

	delete temp_image;

//main_device->getBuilder()->placeObject( main_device, "torus123", 
	//main_device->getMyRoom(), 
	//					  		Kaleidoscope::Vec3D( 0.0f, 0.0f, 0.0f ), 
	//					  		Kaleidoscope::Vec3D( 1.0f, 1.0f, 1.0f ), 
	//					  		Kaleidoscope::Vec3D( 0.0f, 0.0f, 0.0f )	);
	//main_device->getLoader()->load3ds( main_device, "dongs", "../media/torus.3ds", true );


	
    // Main rendering loop
    do
    {
		main_device->run();

        // Swap front and back buffers (we use a double buffered display)
		SDL_GL_SwapBuffers();
	}
    while( main_device->running );
	
	
	// Cleanup	
	delete main_device; // Also deletes renderer, cam, interface, loader, etc
	
	TTF_Quit();
	SDLNet_Quit();
	SDL_Quit();
	
    return 0;
}





