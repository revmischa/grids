/*
 *  builder.cpp
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 8/20/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <kaleidoscope/builder.h>

#include <grids/define.h>

#include <math.h>



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
		//

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
		else if( false )
		{
			int world_size = 10; // 20 x 20 x 20 = 8000 total rooms

			int room_width = d->room_width;

			float dist_const = sqrt( 3 * (room_width * room_width ) );

			Vec3D closest_position = Vec3D( -world_size * room_width,
											-world_size * room_width,
											-world_size * room_width		);

			Vec3D temp_position;

			for( int i = -world_size; i < world_size; i++)
			{
				for( int g = -world_size; g < world_size; g++)
				{
					for( int h = -world_size; h < world_size; h++)
					{
						for( int j = 0; j < d->world_hash[ "rooms" ].size(); j++ )
						{
							GridsID temp_id = d->world_hash[ "rooms" ][ j ].asString();

							Vec3D temp_room_position = Vec3D(	d->world_hash[ temp_id ][ "position" ][ 0u ].asDouble(),
																d->world_hash[ temp_id ][ "position" ][ 1u ].asDouble(),
																d->world_hash[ temp_id ][ "position" ][ 2u ].asDouble()	);


							if( sqrt(	( room_width * i -  temp_room_position.X ) * ( room_width * i ) +
										( room_width * g) * ( room_width * g) +
										( room_width * h) * ( room_width * h)	)  )


							{

							}

						} // end for Rooms
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

		else
		{
			int num_rooms = d->world_hash[ "rooms" ].size();
			float room_width = d->room_width;

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
	}



	void Builder::buildRoom( Device * d, GridsID new_id )
	{
		d->world_hash[ new_id ][ "quads" ] = Grids::Value();
		d->world_hash[ new_id ][ "lines" ] = Grids::Value();

		d->world_hash[ new_id ][ "lines" ][ 0u ] = Grids::Value() ;

		d->world_hash[ new_id ][ "color" ] = Grids::Value() ;

		// Lines Color
		d->world_hash[ new_id ][ "color" ][ 0u ] = Grids::Value();
		d->world_hash[ new_id ][ "color" ][ 0u ][ 0u ] = 0.0f;
		d->world_hash[ new_id ][ "color" ][ 0u ][ 1u ] = 1.0f;
		d->world_hash[ new_id ][ "color" ][ 0u ][ 2u ] = 0.0f;
		d->world_hash[ new_id ][ "color" ][ 0u ][ 3u ] = 0.5f;

		d->world_hash[ new_id ][ "lines" ][ 0u ][ "indices" ] = Grids::Value();
		d->world_hash[ new_id ][ "lines" ][ 0u ][ "color" ] = 0u;

		d->world_hash[ new_id ][ "vertices" ] = Grids::Value();

		int num_lines = d->room_width;
		float room_width = d->room_width;

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
		d->world_hash[ new_id ][ "color" ][ 1u ][ 0u ] = 0.0f;
		d->world_hash[ new_id ][ "color" ][ 1u ][ 1u ] = 1.0f;
		d->world_hash[ new_id ][ "color" ][ 1u ][ 2u ] = 0.0f;
		d->world_hash[ new_id ][ "color" ][ 1u ][ 3u ] = 0.05f;

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


	}

	void Builder::placeObject( Device * d, GridsID new_id, GridsID Room_ID, Vec3D new_position, Vec3D new_scale, Vec3D new_rotation )
	{

		if( !( d->world_hash[ Room_ID ][ "objects" ] ) )
		{
			d->world_hash[ Room_ID ][ "objects" ][ 0u ] = new_id;
		}
		else
		{
			int temp_index = d->world_hash[ Room_ID ][ "objects" ].size();

			d->world_hash[ Room_ID ][ "objects" ][ temp_index ] = new_id;
		}

		d->world_hash[ new_id ][ "room" ] = Room_ID;

		d->world_hash[ new_id ][ "position" ] = Grids::Value();
		d->world_hash[ new_id ][ "position" ][ 0u ] = new_position.X;
		d->world_hash[ new_id ][ "position" ][ 1u ] = new_position.Y;
		d->world_hash[ new_id ][ "position" ][ 2u ] = new_position.Z;

		d->world_hash[ new_id ][ "scale" ] = Grids::Value();
		d->world_hash[ new_id ][ "scale" ][ 0u ] = new_scale.X;
		d->world_hash[ new_id ][ "scale" ][ 1u ] = new_scale.Y;
		d->world_hash[ new_id ][ "scale" ][ 2u ] = new_scale.Z;

		d->world_hash[ new_id ][ "rotation" ] = Grids::Value();
		d->world_hash[ new_id ][ "rotation" ][ 0u ] = new_rotation.X;
		d->world_hash[ new_id ][ "rotation" ][ 1u ] = new_rotation.Y;
		d->world_hash[ new_id ][ "rotation" ][ 2u ] = new_rotation.Z;

	}

	void Builder::placeObject( Device * d, GridsID new_id, Vec3D new_position )
	// Place an object that's not contained inside a room
	{

	}

	void Builder::buildChair( Device * d, GridsID new_id )
	{
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
	}

	void Builder::buildBox( Device * d, GridsID new_id, float box_size, float * color )
	{
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

	}

} // end namespace Kaleidoscope

