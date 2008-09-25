/*
 *  builder.cpp
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 8/20/08.
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

#include <kaleidoscope/builder.h>
#include <kaleidoscope/simpleCube.h>

#include <grids/define.h>

#include <math.h>
#include <SDL_image/SDL_image.h>



namespace Kaleidoscope
{

	Builder::Builder()
	{

	}

	void Builder::placeRoom( Device * d, GridsID new_id )
	{
		//	See how many other rooms exist
		//		if none, place at center
		//		if many, randomly position room in the closest position
		
		std::cout << "Placing room" << std::endl;
		
		float room_width = d->getRoomWidth();
		
		d->lockWorldHash();
	
		if( !( d->world_hash[ "rooms" ] ) )
		{
			d->world_hash[ "rooms" ] = Grids::Value();
			d->world_hash[ "rooms" ][ 0u ] = new_id;

			d->world_hash[ new_id ] = Grids::Value();
			d->world_hash[ new_id ][ "position" ][ 0u ] = 0.0f;
			d->world_hash[ new_id ][ "position" ][ 1u ] = 0.0f;
			d->world_hash[ new_id ][ "position" ][ 2u ] = 0.0f;
			d->world_hash[ new_id ][ "scale" ][ 0u ] = 1.0f;
			d->world_hash[ new_id ][ "scale" ][ 1u ] = 1.0f;
			d->world_hash[ new_id ][ "scale" ][ 2u ] = 1.0f;
			d->world_hash[ new_id ][ "rotation" ][ 0u ] = 0.0f;
			d->world_hash[ new_id ][ "rotation" ][ 1u ] = 0.0f;
			d->world_hash[ new_id ][ "rotation" ][ 2u ] = 0.0f;
		}
		else
		{
			int world_size = 5; // 20 x 20 x 20 = 8000 total rooms
			
			// The default position is the position to place new rooms if all of the rooms are filled up
			// It is the farthest possible position
			Vec3D default_position = Vec3D( -world_size * room_width * 2,
										   -world_size * room_width * 2,
										   -world_size * room_width * 2		);
			
			Vec3D closest_position = default_position; 

			Vec3D temp_position;
			
			for( int i = -world_size; i < world_size; i++)
			{
				for( int g = -world_size; g < world_size; g++)
				{
					for( int h = -world_size; h < world_size; h++)
					{
						temp_position = Vec3D( i * 2 * room_width, g * 2 * room_width, h * 2 * room_width );
						
						for( int j = 0; j < d->world_hash[ "rooms" ].size(); j++ )
						{
							GridsID temp_id = d->world_hash[ "rooms" ][ j ].asString();

							Vec3D temp_room_position = Vec3D(	d->world_hash[ temp_id ][ "position" ][ 0u ].asDouble(),
																d->world_hash[ temp_id ][ "position" ][ 1u ].asDouble(),
																d->world_hash[ temp_id ][ "position" ][ 2u ].asDouble()	);
							
							if( temp_position == temp_room_position )
							{
								temp_position = default_position;
							}
							
						} // end for Rooms
						
						
						if( temp_position.getLength() < closest_position.getLength() )
						{
							closest_position = temp_position;
						}
						
						
					} // end for h
				} // end for g
			} // end for i

			int num_rooms = d->world_hash[ "rooms" ].size();

			d->world_hash[ "rooms" ][ num_rooms ] = new_id;

			d->world_hash[ new_id ] = Grids::Value();

			d->world_hash[ new_id ][ "position" ][ 0u ] = closest_position.X;
			d->world_hash[ new_id ][ "position" ][ 1u ] = closest_position.Y;
			d->world_hash[ new_id ][ "position" ][ 2u ] = closest_position.Z;
			d->world_hash[ new_id ][ "scale" ][ 0u ] = 1.0f;
			d->world_hash[ new_id ][ "scale" ][ 1u ] = 1.0f;
			d->world_hash[ new_id ][ "scale" ][ 2u ] = 1.0f;
			d->world_hash[ new_id ][ "rotation" ][ 0u ] = 0.0f;
			d->world_hash[ new_id ][ "rotation" ][ 1u ] = 0.0f;
			d->world_hash[ new_id ][ "rotation" ][ 2u ] = 0.0f;
		}

		if( false )
		{
			int num_rooms = d->world_hash[ "rooms" ].size();
			
			

			d->world_hash[ "rooms" ][ num_rooms ] = new_id;

			d->world_hash[ new_id ] = Grids::Value();

			d->world_hash[ new_id ][ "width" ] = d->room_width;

			d->world_hash[ new_id ][ "position" ][ 0u ] = num_rooms * 2 * room_width ;
			d->world_hash[ new_id ][ "position" ][ 1u ] = 0.0f;
			d->world_hash[ new_id ][ "position" ][ 2u ] = 0.0f;
			d->world_hash[ new_id ][ "scale" ][ 0u ] = 1.0f;
			d->world_hash[ new_id ][ "scale" ][ 1u ] = 1.0f;
			d->world_hash[ new_id ][ "scale" ][ 2u ] = 1.0f;
			d->world_hash[ new_id ][ "rotation" ][ 0u ] = 0.0f;
			d->world_hash[ new_id ][ "rotation" ][ 1u ] = 0.0f;
			d->world_hash[ new_id ][ "rotation" ][ 2u ] = 0.0f;

		}
		
		d->unlockWorldHash();
		
	} // end placeRoom



	void Builder::buildRoom( Device * d, GridsID new_id )
	{
		float room_width = d->getRoomWidth();
		int num_lines = room_width;
		
		d->lockWorldHash();
		
		d->world_hash[ new_id ][ "quads" ] = Grids::Value();
		d->world_hash[ new_id ][ "lines" ] = Grids::Value();

		d->world_hash[ new_id ][ "lines" ][ 0u ] = Grids::Value() ;

		d->world_hash[ new_id ][ "color" ] = Grids::Value() ;

		// Lines Color
		d->world_hash[ new_id ][ "color" ][ 0u ] = Grids::Value();
		d->world_hash[ new_id ][ "color" ][ 0u ][ 0u ] = 1.0f;
		d->world_hash[ new_id ][ "color" ][ 0u ][ 1u ] = 1.0f;
		d->world_hash[ new_id ][ "color" ][ 0u ][ 2u ] = 1.0f;
		d->world_hash[ new_id ][ "color" ][ 0u ][ 3u ] = 0.1f;

		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ] = Grids::Value();
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "color" ] = 0u;

		d->world_hash[ new_id ][ "vertices" ] = Grids::Value();



		for(int i = 0; i <= num_lines * 2; i += 4 ) // i * 2 from -num_lines to num_lines --  + 4 Vertices
		{
			d->world_hash[ new_id ][ "vertices" ][ i ] = Grids::Value();

			d->world_hash[ new_id ][ "vertices" ][ i ][ 0u ] = (float)i - room_width;
			d->world_hash[ new_id ][ "vertices" ][ i ][ 1u ] = 0.0f;
			d->world_hash[ new_id ][ "vertices" ][ i ][ 2u ] = (float)-num_lines;

			d->world_hash[ new_id ][ "vertices" ][ i+1 ] = Grids::Value();

			d->world_hash[ new_id ][ "vertices" ][ i+1 ][ 0u ] = (float)i - room_width;
			d->world_hash[ new_id ][ "vertices" ][ i+1 ][ 1u ] = 0.0f;
			d->world_hash[ new_id ][ "vertices" ][ i+1 ][ 2u ] = (float)num_lines ;

			d->world_hash[ new_id ][ "vertices" ][ i+2 ] = Grids::Value();

			d->world_hash[ new_id ][ "vertices" ][ i+2 ][ 0u ] = (float)num_lines;
			d->world_hash[ new_id ][ "vertices" ][ i+2 ][ 1u ] = 0.0f;
			d->world_hash[ new_id ][ "vertices" ][ i+2 ][ 2u ] = (float)i - room_width;

			d->world_hash[ new_id ][ "vertices" ][ i+3 ] = Grids::Value();

			d->world_hash[ new_id ][ "vertices" ][ i+3 ][ 0u ] = (float)-num_lines;
			d->world_hash[ new_id ][ "vertices" ][ i+3 ][ 1u ] = 0.0f;
			d->world_hash[ new_id ][ "vertices" ][ i+3 ][ 2u ] = (float)i - room_width;
		}

		// Indices are ordered pairs ( or triplets, or quintuplets ) of numbers that indicate whicu vertices belong to which line, triangle, or quadrilateral

		int j = 0;

		for( int i = 0; i < num_lines + 1 ; i += 2 )
		{
			d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ i ] = Grids::Value();

			d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ i ][ 0u ] = j;
			d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ i ][ 1u ] = j + 1;

			d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ i + 1 ] = Grids::Value();

			d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ i + 1 ][ 0u ] = j + 2;
			d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ i + 1 ][ 1u ] = j + 3;

			j += 4;
		}

		// Wall Color

		d->world_hash[ new_id ][ "color" ][ 1u ] = Grids::Value();
		d->world_hash[ new_id ][ "color" ][ 1u ][ 0u ] = 1.0f;
		d->world_hash[ new_id ][ "color" ][ 1u ][ 1u ] = 1.0f;
		d->world_hash[ new_id ][ "color" ][ 1u ][ 2u ] = 1.0f;
		d->world_hash[ new_id ][ "color" ][ 1u ][ 3u ] = 0.01f;

		// Outline Color -- faint white
		d->world_hash[ new_id ][ "color" ][ 2u ] = Grids::Value();
		d->world_hash[ new_id ][ "color" ][ 2u ][ 0u ] = 1.0f;
		d->world_hash[ new_id ][ "color" ][ 2u ][ 1u ] = 1.0f;
		d->world_hash[ new_id ][ "color" ][ 2u ][ 2u ] = 1.0f;
		d->world_hash[ new_id ][ "color" ][ 2u ][ 3u ] = 0.15f;

		d->world_hash[ new_id ][ "quads" ][ 0u ] = Grids::Value();
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ] = Grids::Value();
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "color" ] = 1; // Use color # 1 to shade this quad

		int k = d->world_hash[ new_id ][ "vertices" ].size();

		for( int i = k; i < k + 8; i++ )
		{
			d->world_hash[ new_id ][ "vertices" ][ i ] = Grids::Value();
		}

		d->world_hash[ new_id ][ "vertices" ][ k ][ 0u ] = -room_width;
		d->world_hash[ new_id ][ "vertices" ][ k ][ 1u ] = -room_width;
		d->world_hash[ new_id ][ "vertices" ][ k ][ 2u ] = -room_width;

		d->world_hash[ new_id ][ "vertices" ][ k + 1 ][ 0u ] = room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 1 ][ 1u ] = -room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 1 ][ 2u ] = -room_width;

		d->world_hash[ new_id ][ "vertices" ][ k + 2 ][ 0u ] = room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 2 ][ 1u ] = room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 2 ][ 2u ] = -room_width;

		d->world_hash[ new_id ][ "vertices" ][ k + 3 ][ 0u ] = -room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 3 ][ 1u ] = room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 3 ][ 2u ] = -room_width;

		d->world_hash[ new_id ][ "vertices" ][ k + 4 ][ 0u ] = -room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 4 ][ 1u ] = -room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 4 ][ 2u ] = room_width;

		d->world_hash[ new_id ][ "vertices" ][ k + 5 ][ 0u ] = room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 5 ][ 1u ] = -room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 5 ][ 2u ] = room_width;

		d->world_hash[ new_id ][ "vertices" ][ k + 6 ][ 0u ] = room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 6 ][ 1u ] = room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 6 ][ 2u ] = room_width;

		d->world_hash[ new_id ][ "vertices" ][ k + 7 ][ 0u ] = -room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 7 ][ 1u ] = room_width;
		d->world_hash[ new_id ][ "vertices" ][ k + 7 ][ 2u ] = room_width;

		for( int i = 0; i < 6; i++ )
		{
			d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ i ] = Grids::Value();
		}

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 0u ] = k + 0u; // Box top
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 1u ] = k + 1u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 2u ] = k + 2u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 3u ] = k + 3u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 0u ] = k + 4u; // Box bottom
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 1u ] = k + 5u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 2u ] = k + 6u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 3u ] = k + 7u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 2u ][ 0u ] = k + 4u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 2u ][ 1u ] = k + 5u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 2u ][ 2u ] = k + 1u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 2u ][ 3u ] = k + 0u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 3u ][ 0u ] = k + 7u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 3u ][ 1u ] = k + 6u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 3u ][ 2u ] = k + 2u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 3u ][ 3u ] = k + 3u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 4u ][ 0u ] = k + 4u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 4u ][ 1u ] = k + 7u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 4u ][ 2u ] = k + 3u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 4u ][ 3u ] = k + 0u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 5u ][ 0u ] = k + 5u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 5u ][ 1u ] = k + 6u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 5u ][ 2u ] = k + 2u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 5u ][ 3u ] = k + 1u;


		// Do the faint white outline lines

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "color" ] = 2u;

		for( int i = 0; i < 12; i++ )
		{
			d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ i ] = Grids::Value();
		}

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 0u ][ 0u ] = k + 0u;
		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 0u ][ 1u ] = k + 1u;

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 1u ][ 0u ] = k + 3u;
		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 1u ][ 1u ] = k + 2u;

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 2u ][ 0u ] = k + 4u;
		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 2u ][ 1u ] = k + 5u;

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 3u ][ 0u ] = k + 7u;
		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 3u ][ 1u ] = k + 6u;

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 4u ][ 0u ] = k + 0u;
		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 4u ][ 1u ] = k + 3u;

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 5u ][ 0u ] = k + 1u;
		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 5u ][ 1u ] = k + 2u;

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 6u ][ 0u ] = k + 4u;
		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 6u ][ 1u ] = k + 7u;

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 7u ][ 0u ] = k + 5u;
		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 7u ][ 1u ] = k + 6u;

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 8u ][ 0u ] = k + 4u;
		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 8u ][ 1u ] = k + 0u;

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 9u ][ 0u ] = k + 7u;
		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 9u ][ 1u ] = k + 3u;

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 10u ][ 0u ] = k + 5u;
		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 10u ][ 1u ] = k + 1u;

		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 11u ][ 0u ] = k + 6u;
		d->world_hash[ new_id ][ "lines" ][ 1u ][ "indices" ][ 11u ][ 1u ] = k + 2u;
		
		d->unlockWorldHash();
		
	} // end BuildRoom

	void Builder::placeObject( Device * d, GridsID new_id, GridsID Room_ID, Vec3D new_position, Vec3D new_scale, Vec3D new_rotation )
	{
		d->lockWorldHash();

		if( !( d->world_hash[ Room_ID ][ "objects" ] ) )
		{
			d->world_hash[ Room_ID ][ "objects" ][ 0u ] = new_id;
		}
		else
		{
			int temp_index = d->world_hash[ Room_ID ][ "objects" ].size();
			
			bool id_in_room = false;
			
			for( int i = 0; i < temp_index; i++ )
			{
				if( new_id == d->world_hash[ Room_ID ][ "objects" ][ i ].asString() )
				{
					id_in_room = true;
				}
			}
			
			if( !id_in_room ){
				d->world_hash[ Room_ID ][ "objects" ][ temp_index ] = new_id;
			}
		}

		d->world_hash[ new_id ][ "room" ] = Room_ID;

		d->world_hash[ new_id ][ "position" ][ 0u ] = new_position.X;
		d->world_hash[ new_id ][ "position" ][ 1u ] = new_position.Y;
		d->world_hash[ new_id ][ "position" ][ 2u ] = new_position.Z;

		d->world_hash[ new_id ][ "scale" ][ 0u ] = new_scale.X;
		d->world_hash[ new_id ][ "scale" ][ 1u ] = new_scale.Y;
		d->world_hash[ new_id ][ "scale" ][ 2u ] = new_scale.Z;

		d->world_hash[ new_id ][ "rotation" ][ 0u ] = new_rotation.X;
		d->world_hash[ new_id ][ "rotation" ][ 1u ] = new_rotation.Y;
		d->world_hash[ new_id ][ "rotation" ][ 2u ] = new_rotation.Z;
		
		d->lockWorldHash();

	}

	void Builder::placeObject( Device * d, GridsID new_id, Vec3D new_position, Vec3D new_scale, Vec3D new_rotation  )
	// Place an object that's not contained inside a room
	{
		

	}

	void Builder::buildChair( Device * d, GridsID new_id )
	{
		d->lockWorldHash();
		
		d->world_hash[ new_id ][ "quads" ] = Grids::Value();

		d->world_hash[ new_id ][ "color" ] = Grids::Value();
		d->world_hash[ new_id ][ "color" ][ 0u ] = Grids::Value();

		d->world_hash[ new_id ][ "color" ][ 0u ][ 0u ] = 0.9f;
		d->world_hash[ new_id ][ "color" ][ 0u ][ 1u ] = 0.2f;
		d->world_hash[ new_id ][ "color" ][ 0u ][ 2u ] = 0.2f;
		d->world_hash[ new_id ][ "color" ][ 0u ][ 3u ] = 0.75f;

		// Quads have 4 vertices, so 4 more hashes are needed

		d->world_hash[ new_id ][ "vertices" ][ 0u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 0u ][ 0u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 0u ][ 1u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 0u ][ 2u ] = -1.0f;

		d->world_hash[ new_id ][ "vertices" ][ 1u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 1u ][ 0u ] = 1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 1u ][ 1u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 1u ][ 2u ] = -1.0f;

		d->world_hash[ new_id ][ "vertices" ][ 2u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 2u ][ 0u ] = 1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 2u ][ 1u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 2u ][ 2u ] = 1.0f;

		d->world_hash[ new_id ][ "vertices" ][ 3u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 3u ][ 0u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 3u ][ 1u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 3u ][ 2u ] = 1.0f;

		// Quads have 4 vertices, so 4 more hashes are needed

		d->world_hash[ new_id ][ "vertices" ][ 4u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 4u ][ 0u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 4u ][ 1u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 4u ][ 2u ] = -1.3f;

		d->world_hash[ new_id ][ "vertices" ][ 5u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 5u ][ 0u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 5u ][ 1u ] = 1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 5u ][ 2u ] = -1.3f;

		d->world_hash[ new_id ][ "vertices" ][ 6u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 6u ][ 0u ] = 1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 6u ][ 1u ] = 1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 6u ][ 2u ] = -1.3f;

		d->world_hash[ new_id ][ "vertices" ][ 7u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 7u ][ 0u ] = 1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 7u ][ 1u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 7u ][ 2u ] = -1.3f;

		//d->world_hash[ new_id ][ "lines" ] = Grids::Value();

		d->world_hash[ new_id ][ "quads" ][ 0u ] = Grids::Value();
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "color" ] = 0u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ] = Grids::Value();
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ] = Grids::Value();
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ] = Grids::Value();


		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 0u ] = 0u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 1u ] = 1u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 2u ] = 2u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 3u ] = 3u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 0u ] = 4u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 1u ] = 5u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 2u ] = 6u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 3u ] = 7u;
		
		d->unlockWorldHash();
	}
	
	void Builder::buildBillboard( Device * d, GridsID new_id, std::string image_name )
	// Builds and attaches a glow orb to the given GridsID
	{
		SDL_Surface * temp_surface = loadImage( image_name );

		// pack image pixles into json... hmmm
	}
	
	SDL_Surface * Builder::loadImage( std::string image_name )
	{
		return IMG_Load( image_name.c_str() );
	}

	void Builder::buildBox( Device * d, GridsID new_id, float box_size, float * color )
	{
		d->lockWorldHash();
		
		d->world_hash[ new_id ][ "vertices" ][ 0u ][ 0u ] = -box_size;
		d->world_hash[ new_id ][ "vertices" ][ 0u ][ 1u ] = -box_size;
		d->world_hash[ new_id ][ "vertices" ][ 0u ][ 2u ] = -box_size;

		d->world_hash[ new_id ][ "vertices" ][ 1u ][ 0u ] = box_size;
		d->world_hash[ new_id ][ "vertices" ][ 1u ][ 1u ] = -box_size;
		d->world_hash[ new_id ][ "vertices" ][ 1u ][ 2u ] = -box_size;

		d->world_hash[ new_id ][ "vertices" ][ 2u ][ 0u ] = box_size;
		d->world_hash[ new_id ][ "vertices" ][ 2u ][ 1u ] = box_size;
		d->world_hash[ new_id ][ "vertices" ][ 2u ][ 2u ] = -box_size;

		d->world_hash[ new_id ][ "vertices" ][ 3u ][ 0u ] = -box_size;
		d->world_hash[ new_id ][ "vertices" ][ 3u ][ 1u ] = box_size;
		d->world_hash[ new_id ][ "vertices" ][ 3u ][ 2u ] = -box_size;

		d->world_hash[ new_id ][ "vertices" ][ 4u ][ 0u ] = -box_size;
		d->world_hash[ new_id ][ "vertices" ][ 4u ][ 1u ] = -box_size;
		d->world_hash[ new_id ][ "vertices" ][ 4u ][ 2u ] = box_size;

		d->world_hash[ new_id ][ "vertices" ][ 5u ][ 0u ] = box_size;
		d->world_hash[ new_id ][ "vertices" ][ 5u ][ 1u ] = -box_size;
		d->world_hash[ new_id ][ "vertices" ][ 5u ][ 2u ] = box_size;

		d->world_hash[ new_id ][ "vertices" ][ 6u ][ 0u ] = box_size;
		d->world_hash[ new_id ][ "vertices" ][ 6u ][ 1u ] = box_size;
		d->world_hash[ new_id ][ "vertices" ][ 6u ][ 2u ] = box_size;

		d->world_hash[ new_id ][ "vertices" ][ 7u ][ 0u ] = -box_size;
		d->world_hash[ new_id ][ "vertices" ][ 7u ][ 1u ] = box_size;
		d->world_hash[ new_id ][ "vertices" ][ 7u ][ 2u ] = box_size;

		d->world_hash[ new_id ][ "color" ][ 0u ][ 0u ] = color[0];
		d->world_hash[ new_id ][ "color" ][ 0u ][ 1u ] = color[1];
		d->world_hash[ new_id ][ "color" ][ 0u ][ 2u ] = color[2];
		d->world_hash[ new_id ][ "color" ][ 0u ][ 3u ] = color[3];

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "color" ] = 0u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 0u ] = 0u; // Box top
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 1u ] = 1u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 2u ] = 2u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 3u ] = 3u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 0u ] = 4u; // Box bottom
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 1u ] = 5u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 2u ] = 6u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 3u ] = 7u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 2u ][ 0u ] = 4u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 2u ][ 1u ] = 5u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 2u ][ 2u ] = 1u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 2u ][ 3u ] = 0u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 3u ][ 0u ] = 7u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 3u ][ 1u ] = 6u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 3u ][ 2u ] = 2u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 3u ][ 3u ] = 3u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 4u ][ 0u ] = 4u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 4u ][ 1u ] = 7u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 4u ][ 2u ] = 3u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 4u ][ 3u ] = 0u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 5u ][ 0u ] = 5u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 5u ][ 1u ] = 6u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 5u ][ 2u ] = 2u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 5u ][ 3u ] = 1u;
		
		
		// Line Color
		d->world_hash[ new_id ][ "color" ][ 1u ][ 0u ] = 0.75f;
		d->world_hash[ new_id ][ "color" ][ 1u ][ 1u ] = 0.75f;
		d->world_hash[ new_id ][ "color" ][ 1u ][ 2u ] = 0.75f;
		d->world_hash[ new_id ][ "color" ][ 1u ][ 3u ] = 0.15f;
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "color" ] = 1u;
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 0u ][ 0u ] = 0u;
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 0u ][ 1u ] = 1u;
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 1u ][ 0u ] = 1u;
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 1u ][ 1u ] = 2u;
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 2u ][ 0u ] = 2u;
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 2u ][ 1u ] = 3u;
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 3u ][ 0u ] = 3u;
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 3u ][ 1u ] = 0u;
		
		
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 4u ][ 0u ] = 4u;
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 4u ][ 1u ] = 5u;
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 5u ][ 0u ] = 5u;
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 5u ][ 1u ] = 6u;
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 6u ][ 0u ] = 6u;
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 6u ][ 1u ] = 7u;
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 7u ][ 0u ] = 7u;
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 7u ][ 1u ] = 4u;
		
		
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 8u ][ 0u ] = 0u;
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 8u ][ 1u ] = 4u;
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 9u ][ 0u ] = 1u;
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 9u ][ 1u ] = 5u;
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 10u ][ 0u ] = 2u;
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 10u ][ 1u ] = 6u;
		
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 11u ][ 0u ] = 3u;
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ][ 11u ][ 1u ] = 7u;
		
		d->unlockWorldHash();
		
	}
	
	void Builder::createRandomBoxes( Device * d, Grids::GridsID room_id, int num_boxes )
	{
		SimpleCube * inter_cube = new SimpleCube( );
		float temp_box_color[ 4 ];
		
		float room_width = d->getRoomWidth();
		
		for( int i = 0; i < num_boxes; i++ )
		{
			temp_box_color[ 0 ] = (rand() % 10000)/10000.0f;
			temp_box_color[ 1 ] = (rand() % 10000)/10000.0f;
			temp_box_color[ 2 ] = (rand() % 10000)/10000.0f;
			temp_box_color[ 3 ] = 0.35f;
			
			inter_cube->requestCreateCube( d, room_id, Vec3D( room_width - (rand() % 10000)/10000.0f * room_width * 2.0f,  
																									 room_width - (rand() % 10000)/10000.0f * room_width * 2.0f , 
																									 room_width - (rand() % 10000)/10000.0f * room_width * 2.0f ),
										  2.0f, &temp_box_color[ 0 ]  );
		}
		
		delete inter_cube;
	}
	
	
	int Builder::packImage( Device * d, GridsID in_id, SDL_Surface * in_surface )
	{
		if( !( d->world_hash[ in_id ][ "images" ] ) ) // if there are no images attached to the array
		{
			d->world_hash[ in_id ][ "images" ][ 0u ] = Grids::Value();			
			
			int surface_x = in_surface->w;
			int surface_y = in_surface->h;
			
			for( int i = 0; i < surface_x; i++ )
			{
				for( int g = 0; g < surface_y; g++ )
				{
					d->world_hash[ in_id ][ "images" ][ 0u ][ i ][ g ] = getPixel(in_surface, i, g);
				}
			}
			
			return 0;
			
		} // end if "images" is empty
		else
		{
			int new_index = d->world_hash[ in_id ][ "images" ].size();
			
			int surface_x = in_surface->w;
			int surface_y = in_surface->h;
			
			for( int i = 0; i < surface_x; i++ )
			{
				for( int g = 0; g < surface_y; g++ )
				{
					d->world_hash[ in_id ][ "images" ][ new_index ][ i ][ g ] = getPixel(in_surface, i, g);
				}
			}
			
			return new_index;
		}
		
		
	} // end packImage()
	
	
	SDL_Surface * Builder::getImage( Device * d, GridsID in_id )
	{
		return getImage( d, in_id, 0u );
	}
	
	SDL_Surface * Builder::getImage( Device * d, GridsID in_id, int in_index )
	{
		int surface_x = d->world_hash[ in_id ][ "images" ][ in_index ].size();
		int surface_y = d->world_hash[ in_id ][ "images" ][ in_index ][ 0u ].size(); // assuming of course that this as rectangular...
		
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				Uint32 rmask = 0xff000000;
				Uint32 gmask = 0x00ff0000;
				Uint32 bmask = 0x0000ff00;
				Uint32 amask = 0x000000ff;
		#else
				Uint32 rmask = 0x000000ff;
				Uint32 gmask = 0x0000ff00;
				Uint32 bmask = 0x00ff0000;
				Uint32 amask = 0xff000000;
		#endif
				
		SDL_Surface * temp_surface = SDL_CreateRGBSurface( SDL_HWSURFACE, surface_x, surface_y, 32, rmask, gmask, bmask, amask );
		
		SDL_LockSurface( temp_surface );
		
		// modify pixels
		
		Uint32 temp_pixel;
		
		for( int i = 0; i < surface_x; i++ )
		{
			for( int g = 0; g < surface_y; g++ )
			{
				temp_pixel = d->world_hash[ in_id ][ "images" ][ in_index ][ i ][ g ].asDouble();
				
				putPixel( temp_surface, i, g, temp_pixel );
			}
		}
		
		SDL_UnlockSurface( temp_surface );
		
		return temp_surface;
		
	}
	
	
	
	
	
	
	/////////////////////
	///  PRIVATE
	////////////////////
	
	Uint32 Builder::getPixel(SDL_Surface *surface, int x, int y)
	{
		int bpp = surface->format->BytesPerPixel;
		/* Here p is the address to the pixel we want to retrieve */
		Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
		
		switch(bpp) {
			case 1:
				return *p;
				
			case 2:
				return *(Uint16 *)p;
				
			case 3:
				if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
					return p[0] << 16 | p[1] << 8 | p[2];
				else
					return p[0] | p[1] << 8 | p[2] << 16;
				
			case 4:
				return *(Uint32 *)p;
				
			default:
				return 0;       /* shouldn't happen, but avoids warnings */
		}
	}
	
	void Builder::putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
	{
		int bpp = surface->format->BytesPerPixel;
		/* Here p is the address to the pixel we want to set */
		Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
		
		switch(bpp) {
			case 1:
				*p = pixel;
				break;
				
			case 2:
				*(Uint16 *)p = pixel;
				break;
				
			case 3:
				if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
					p[0] = (pixel >> 16) & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = pixel & 0xff;
				} else {
					p[0] = pixel & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = (pixel >> 16) & 0xff;
				}
				break;
				
			case 4:
				*(Uint32 *)p = pixel;
				break;
		}
	}
	
	void Builder::lock( Device * d )
	{
		SDL_LockMutex( d->builder_mutex );
	}
	
	void Builder::unlock( Device * d )
	{
		SDL_UnlockMutex( d->builder_mutex );
	}

} // end namespace Kaleidoscope

