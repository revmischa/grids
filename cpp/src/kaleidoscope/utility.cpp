/*
 *  simpleCube.cpp
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 9/15/08.
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

#include <kaleidoscope/utility.h>

#include <kaleidoscope/device.h>
#include <kaleidoscope/simpleCube.h>

#include <grids/interface.h>
#include <grids/object.h>

#include <iostream>

#include <sstream>

namespace Kaleidoscope
{
	
	Utility::Utility( ) // Device * d, GridsID cube_room, Vec3D cube_position, float side_length, float * cube_color )
	{
		// Notify grids that we're making an object, etc.
		// requestCreateCube( d, cube_room, cube_position, side_length, cube_color );
		
	}
	
	void Utility::requestCreate( Device * d, GridsID utility_room, Vec3D utility_position, Vec2D screen_position  )
	{
		Grids::Value * temp_value = new Grids::Value();
				
		(*temp_value)[ "room_id" ] = utility_room;
		
		(*temp_value)[ "type" ][ "name" ] = "Utility";
		
		// Start all the variables specific to this object type
		(*temp_value)[ "type" ][ "ScreenPosition" ][ 0u ] = screen_position.X;
		(*temp_value)[ "type" ][ "ScreenPosition" ][ 1u ] = screen_position.Y;
				
		// End all the variables specific to this object type
		
		Grids::Object::loadPosition( temp_value, utility_position, Vec3D( 0.0f, 0.0f, 0.0f), Vec3D( 0.0f, 0.0f, 0.0f ) );

		d->getInterface()->getObjectController()->requestCreateObject(d, temp_value );
	}
	

	void Utility::displayRooms( Device * d, Grids::GridsID requesting_room ){
		requestRooms(d, requesting_room);
	}

	void Utility::requestRooms(Device * d, Grids::GridsID requesting_room){
		d->getInterface()->requestServerRooms( requesting_room, "foo" );
	}
	
	void Utility::puts( std::string in_string ){
		std::cout << in_string << std::endl;
	}


	void Utility::puts( double in_var  ){
		std::cout << in_var << std::endl;
	}


	void Utility::puts( std::string in_var1, std::string in_var2  ){
		std::cout << in_var1 << " " << in_var2 << std::endl;
	}
	void Utility::puts( int in_var1, std::string in_var2  ){
		std::cout << in_var1 << " " << in_var2 << std::endl;
	}
	void Utility::puts( std::string in_var1, int in_var2  ){
		std::cout << in_var1 << " " << in_var2 << std::endl;
	}

	void Utility::puts( Vec3D in_vec ){
		std::cout << "< " << in_vec.X << " : " << in_vec.Y << " : " << in_vec.Z << " >" <<std::endl;
	}
				

	void Utility::puts( std::string in_str, Vec3D in_vec ){
		std::cout << in_str << "  < " << in_vec.X << " : " << in_vec.Y << " : " << in_vec.Z << " >" <<std::endl;
	}
				

} // end namespace Kaleidoscope

