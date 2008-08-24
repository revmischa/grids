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
		
		if( !( d->world_hash[ "Rooms" ] ) )
		{
			d->world_hash[ "Rooms" ] = Grids::complex_type();
			d->world_hash[ "Rooms" ][ 0u ] = new_id;
			
			d->world_hash[ new_id ] = Grids::complex_type();
			d->world_hash[ new_id ][ "Position" ][ "x" ] = 0.0f;
			d->world_hash[ new_id ][ "Position" ][ "y" ] = 0.0f;
			d->world_hash[ new_id ][ "Position" ][ "z" ] = 0.0f;
			d->world_hash[ new_id ][ "Scale" ][ "x" ] = 1.0f;
			d->world_hash[ new_id ][ "Scale" ][ "y" ] = 1.0f;
			d->world_hash[ new_id ][ "Scale" ][ "z" ] = 1.0f;
			d->world_hash[ new_id ][ "Rotation" ][ "x" ] = 0.0f;
			d->world_hash[ new_id ][ "Rotation" ][ "y" ] = 0.0f;
			d->world_hash[ new_id ][ "Rotation" ][ "z" ] = 0.0f;
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
						for( int j = 0; j < d->world_hash[ "Rooms" ].size(); j++ )
						{
							GridsID temp_id = d->world_hash[ "Rooms" ][ j ].asString();
							
							Vec3D temp_room_position = Vec3D(	d->world_hash[ temp_id ][ "Position" ][ "x" ].asDouble(), 
																d->world_hash[ temp_id ][ "Position" ][ "y" ].asDouble(),
																d->world_hash[ temp_id ][ "Position" ][ "z" ].asDouble()	);
							
							
							if( sqrt(	( room_width * i -  temp_room_position.X ) * ( room_width * i ) + 
										( room_width * g) * ( room_width * g) + 
										( room_width * h) * ( room_width * h)	)  )
								
							
							{
							
							}
							
						} // end for Rooms
					} // end for h
				} // end for g
			} // end for i
			
			int num_rooms = d->world_hash[ "Rooms" ].size();
			
			d->world_hash[ "Rooms" ][ num_rooms ] = new_id;
			
			d->world_hash[ new_id ] = Grids::complex_type();
			
			d->world_hash[ new_id ][ "Position" ][ "x" ] = closest_position.X;
			d->world_hash[ new_id ][ "Position" ][ "y" ] = closest_position.Y;
			d->world_hash[ new_id ][ "Position" ][ "z" ] = closest_position.Z;
			d->world_hash[ new_id ][ "Scale" ][ "x" ] = 1.0f;
			d->world_hash[ new_id ][ "Scale" ][ "y" ] = 1.0f;
			d->world_hash[ new_id ][ "Scale" ][ "z" ] = 1.0f;
			d->world_hash[ new_id ][ "Rotation" ][ "x" ] = 0.0f;
			d->world_hash[ new_id ][ "Rotation" ][ "y" ] = 0.0f;
			d->world_hash[ new_id ][ "Rotation" ][ "z" ] = 0.0f;
		}
		
		else
		{
			int num_rooms = d->world_hash[ "Rooms" ].size();
			float room_width = d->room_width;
		
			d->world_hash[ "Rooms" ][ num_rooms ] = new_id;
			
			d->world_hash[ new_id ] = Grids::complex_type();
			
			d->world_hash[ new_id ][ "Width" ] = d->room_width;
			
			d->world_hash[ new_id ][ "Position" ][ "x" ] = num_rooms * 2 * room_width ;
			d->world_hash[ new_id ][ "Position" ][ "y" ] = 0.0f;
			d->world_hash[ new_id ][ "Position" ][ "z" ] = 0.0f;
			d->world_hash[ new_id ][ "Scale" ][ "x" ] = 1.0f;
			d->world_hash[ new_id ][ "Scale" ][ "y" ] = 1.0f;
			d->world_hash[ new_id ][ "Scale" ][ "z" ] = 1.0f;
			d->world_hash[ new_id ][ "Rotation" ][ "x" ] = 0.0f;
			d->world_hash[ new_id ][ "Rotation" ][ "y" ] = 0.0f;
			d->world_hash[ new_id ][ "Rotation" ][ "z" ] = 0.0f;
			
		}
	}
	
	
	
	void Builder::buildRoom( Device * d, GridsID new_id )
	{
		d->world_hash[ new_id ][ "Quads" ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Lines" ] = Grids::complex_type();
		
		d->world_hash[ new_id ][ "Lines" ][ 0u ] = Grids::complex_type() ;
	
		d->world_hash[ new_id ][ "Color" ] = Grids::complex_type() ;
		
		// Lines Color
		d->world_hash[ new_id ][ "Color" ][ 0u ] = Grids::complex_type(); 
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "r" ] = 0.0f;
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "g" ] = 1.0f;
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "b" ] = 0.0f;
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "a" ] = 0.5f;
		
		d->world_hash[ new_id ][ "Lines" ][ 0u ][ "Indices" ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Lines" ][ 0u ][ "Color" ] = 0u;
		
		d->world_hash[ new_id ][ "Vertices" ] = Grids::complex_type();
		
		int num_lines = d->room_width;
		float room_width = d->room_width;
	
		for(int i = 0; i <= num_lines * 2; i += 4 ) // i * 2 from -num_lines to num_lines --  + 4 Vertices 
		{
			d->world_hash[ new_id ][ "Vertices" ][ i ] = Grids::complex_type();
			
			d->world_hash[ new_id ][ "Vertices" ][ i ][ "x" ] = (float)i - room_width;
			d->world_hash[ new_id ][ "Vertices" ][ i ][ "y" ] = 0.0f;
			d->world_hash[ new_id ][ "Vertices" ][ i ][ "z" ] = (float)-num_lines;
			
			d->world_hash[ new_id ][ "Vertices" ][ i+1 ] = Grids::complex_type();
			
			d->world_hash[ new_id ][ "Vertices" ][ i+1 ][ "x" ] = (float)i - room_width;
			d->world_hash[ new_id ][ "Vertices" ][ i+1 ][ "y" ] = 0.0f;
			d->world_hash[ new_id ][ "Vertices" ][ i+1 ][ "z" ] = (float)num_lines ;
			
			d->world_hash[ new_id ][ "Vertices" ][ i+2 ] = Grids::complex_type();
			
			d->world_hash[ new_id ][ "Vertices" ][ i+2 ][ "x" ] = (float)num_lines;
			d->world_hash[ new_id ][ "Vertices" ][ i+2 ][ "y" ] = 0.0f;
			d->world_hash[ new_id ][ "Vertices" ][ i+2 ][ "z" ] = (float)i - room_width;
			
			d->world_hash[ new_id ][ "Vertices" ][ i+3 ] = Grids::complex_type();
			
			d->world_hash[ new_id ][ "Vertices" ][ i+3 ][ "x" ] = (float)-num_lines;
			d->world_hash[ new_id ][ "Vertices" ][ i+3 ][ "y" ] = 0.0f;
			d->world_hash[ new_id ][ "Vertices" ][ i+3 ][ "z" ] = (float)i - room_width;
		}
		
		// Indices are ordered pairs ( or triplets, or quintuplets ) of numbers that indicate whicu vertices belong to which line, triangle, or quadrilateral
		
		int j = 0;
		
		for( int i = 0; i < num_lines + 1 ; i += 2 )
		{
			d->world_hash[ new_id ][ "Lines" ][ 0u ][ "Indices" ][ i ] = Grids::complex_type();
			
			d->world_hash[ new_id ][ "Lines" ][ 0u ][ "Indices" ][ i ][ 0u ] = j;
			d->world_hash[ new_id ][ "Lines" ][ 0u ][ "Indices" ][ i ][ 1u ] = j + 1;
			
			d->world_hash[ new_id ][ "Lines" ][ 0u ][ "Indices" ][ i + 1 ] = Grids::complex_type();
			
			d->world_hash[ new_id ][ "Lines" ][ 0u ][ "Indices" ][ i + 1 ][ 0u ] = j + 2;
			d->world_hash[ new_id ][ "Lines" ][ 0u ][ "Indices" ][ i + 1 ][ 1u ] = j + 3;
			
			j += 4;
		}
		
		// Wall Color
		
		d->world_hash[ new_id ][ "Color" ][ 1u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Color" ][ 1u ][ "r" ] = 0.0f;
		d->world_hash[ new_id ][ "Color" ][ 1u ][ "g" ] = 1.0f;
		d->world_hash[ new_id ][ "Color" ][ 1u ][ "b" ] = 0.0f;
		d->world_hash[ new_id ][ "Color" ][ 1u ][ "a" ] = 0.05f;
		
		// Outline Color -- faint white
		d->world_hash[ new_id ][ "Color" ][ 2u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Color" ][ 2u ][ "r" ] = 1.0f;
		d->world_hash[ new_id ][ "Color" ][ 2u ][ "g" ] = 1.0f;
		d->world_hash[ new_id ][ "Color" ][ 2u ][ "b" ] = 1.0f;
		d->world_hash[ new_id ][ "Color" ][ 2u ][ "a" ] = 0.15f;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Color" ] = 1; // Use color # 1 to shade this quad
			
		int k = d->world_hash[ new_id ][ "Vertices" ].size();
		
		for( int i = k; i < k + 8; i++ )
		{
			d->world_hash[ new_id ][ "Vertices" ][ i ] = Grids::complex_type();
		}
		
		d->world_hash[ new_id ][ "Vertices" ][ k ][ "x" ] = -room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k ][ "y" ] = -room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k ][ "z" ] = -room_width;
		
		d->world_hash[ new_id ][ "Vertices" ][ k + 1 ][ "x" ] = room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 1 ][ "y" ] = -room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 1 ][ "z" ] = -room_width;
		
		d->world_hash[ new_id ][ "Vertices" ][ k + 2 ][ "x" ] = room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 2 ][ "y" ] = room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 2 ][ "z" ] = -room_width;
		
		d->world_hash[ new_id ][ "Vertices" ][ k + 3 ][ "x" ] = -room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 3 ][ "y" ] = room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 3 ][ "z" ] = -room_width;
		
		d->world_hash[ new_id ][ "Vertices" ][ k + 4 ][ "x" ] = -room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 4 ][ "y" ] = -room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 4 ][ "z" ] = room_width;
		
		d->world_hash[ new_id ][ "Vertices" ][ k + 5 ][ "x" ] = room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 5 ][ "y" ] = -room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 5 ][ "z" ] = room_width;
		
		d->world_hash[ new_id ][ "Vertices" ][ k + 6 ][ "x" ] = room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 6 ][ "y" ] = room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 6 ][ "z" ] = room_width;
		
		d->world_hash[ new_id ][ "Vertices" ][ k + 7 ][ "x" ] = -room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 7 ][ "y" ] = room_width;
		d->world_hash[ new_id ][ "Vertices" ][ k + 7 ][ "z" ] = room_width;
		
		for( int i = 0; i < 6; i++ )
		{
			d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ i ] = Grids::complex_type();
		}
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ][ 0u ] = k + 0u; // Box top
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ][ 1u ] = k + 1u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ][ 2u ] = k + 2u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ][ 3u ] = k + 3u;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ][ 0u ] = k + 4u; // Box bottom
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ][ 1u ] = k + 5u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ][ 2u ] = k + 6u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ][ 3u ] = k + 7u;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 2u ][ 0u ] = k + 4u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 2u ][ 1u ] = k + 5u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 2u ][ 2u ] = k + 1u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 2u ][ 3u ] = k + 0u;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 3u ][ 0u ] = k + 7u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 3u ][ 1u ] = k + 6u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 3u ][ 2u ] = k + 2u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 3u ][ 3u ] = k + 3u;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 4u ][ 0u ] = k + 4u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 4u ][ 1u ] = k + 7u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 4u ][ 2u ] = k + 3u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 4u ][ 3u ] = k + 0u;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 5u ][ 0u ] = k + 5u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 5u ][ 1u ] = k + 6u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 5u ][ 2u ] = k + 2u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 5u ][ 3u ] = k + 1u;
		
		
		// Do the faint white outline lines
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Color" ] = 2u;
		
		for( int i = 0; i < 12; i++ )
		{
			d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ i ] = Grids::complex_type();
		}
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 0u ][ 0u ] = k + 0u;
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 0u ][ 1u ] = k + 1u;
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 1u ][ 0u ] = k + 3u;
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 1u ][ 1u ] = k + 2u;
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 2u ][ 0u ] = k + 4u;
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 2u ][ 1u ] = k + 5u;
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 3u ][ 0u ] = k + 7u;
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 3u ][ 1u ] = k + 6u;
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 4u ][ 0u ] = k + 0u;
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 4u ][ 1u ] = k + 3u;
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 5u ][ 0u ] = k + 1u;
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 5u ][ 1u ] = k + 2u;
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 6u ][ 0u ] = k + 4u;
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 6u ][ 1u ] = k + 7u;
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 7u ][ 0u ] = k + 5u;
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 7u ][ 1u ] = k + 6u;
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 8u ][ 0u ] = k + 4u;
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 8u ][ 1u ] = k + 0u;
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 9u ][ 0u ] = k + 7u;
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 9u ][ 1u ] = k + 3u;
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 10u ][ 0u ] = k + 5u;
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 10u ][ 1u ] = k + 1u;
		
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 11u ][ 0u ] = k + 6u;
		d->world_hash[ new_id ][ "Lines" ][ 1u ][ "Indices" ][ 11u ][ 1u ] = k + 2u;

	
	}
	
	void Builder::placeObject( Device * d, GridsID new_id, GridsID Room_ID, Vec3D new_position, Vec3D new_scale, Vec3D new_rotation )
	{
		
		if( !( d->world_hash[ Room_ID ][ "Objects" ] ) )
		{
			d->world_hash[ Room_ID ][ "Objects" ][ 0u ] = new_id;
		}
		else
		{
			int temp_index = d->world_hash[ Room_ID ][ "Objects" ].size();
			
			d->world_hash[ Room_ID ][ "Objects" ][ temp_index ] = new_id;
		}
		
		d->world_hash[ new_id ][ "Room" ] = Room_ID;
	
		d->world_hash[ new_id ][ "Position" ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Position" ][ "x" ] = new_position.X;
		d->world_hash[ new_id ][ "Position" ][ "y" ] = new_position.Y;
		d->world_hash[ new_id ][ "Position" ][ "z" ] = new_position.Z;
		
		d->world_hash[ new_id ][ "Scale" ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Scale" ][ "x" ] = new_scale.X;
		d->world_hash[ new_id ][ "Scale" ][ "y" ] = new_scale.Y;
		d->world_hash[ new_id ][ "Scale" ][ "z" ] = new_scale.Z;
		
		d->world_hash[ new_id ][ "Rotation" ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Rotation" ][ "x" ] = new_rotation.X;
		d->world_hash[ new_id ][ "Rotation" ][ "y" ] = new_rotation.Y;
		d->world_hash[ new_id ][ "Rotation" ][ "z" ] = new_rotation.Z;
		
	}
	
	void Builder::placeObject( Device * d, GridsID new_id, Vec3D new_position )
	// Place an object that's not contained inside a room
	{
	
	}
	
	void Builder::buildChair( Device * d, GridsID new_id )
	{
		d->world_hash[ new_id ][ "Quads" ] = Grids::complex_type();
		 
		d->world_hash[ new_id ][ "Color" ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Color" ][ 0u ] = Grids::complex_type();
		
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "r" ] = 0.9f;
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "g" ] = 0.2f;
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "b" ] = 0.2f;
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "a" ] = 0.75f;
		
		// Quads have 4 vertices, so 4 more hashes are needed
		
		d->world_hash[ new_id ][ "Vertices" ][ 0u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Vertices" ][ 0u ][ "x" ] = -1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 0u ][ "y" ] = -1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 0u ][ "z" ] = -1.0f;
		
		d->world_hash[ new_id ][ "Vertices" ][ 1u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Vertices" ][ 1u ][ "x" ] = 1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 1u ][ "y" ] = -1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 1u ][ "z" ] = -1.0f;
		
		d->world_hash[ new_id ][ "Vertices" ][ 2u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Vertices" ][ 2u ][ "x" ] = 1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 2u ][ "y" ] = -1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 2u ][ "z" ] = 1.0f;
		
		d->world_hash[ new_id ][ "Vertices" ][ 3u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Vertices" ][ 3u ][ "x" ] = -1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 3u ][ "y" ] = -1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 3u ][ "z" ] = 1.0f;

		// Quads have 4 vertices, so 4 more hashes are needed
		
		d->world_hash[ new_id ][ "Vertices" ][ 4u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Vertices" ][ 4u ][ "x" ] = -1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 4u ][ "y" ] = -1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 4u ][ "z" ] = -1.3f;
		
		d->world_hash[ new_id ][ "Vertices" ][ 5u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Vertices" ][ 5u ][ "x" ] = -1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 5u ][ "y" ] = 1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 5u ][ "z" ] = -1.3f;
		
		d->world_hash[ new_id ][ "Vertices" ][ 6u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Vertices" ][ 6u ][ "x" ] = 1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 6u ][ "y" ] = 1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 6u ][ "z" ] = -1.3f;
		
		d->world_hash[ new_id ][ "Vertices" ][ 7u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Vertices" ][ 7u ][ "x" ] = 1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 7u ][ "y" ] = -1.0f;
		d->world_hash[ new_id ][ "Vertices" ][ 7u ][ "z" ] = -1.3f;
		
		//d->world_hash[ new_id ][ "Lines" ] = Grids::complex_type();
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Color" ] = 0u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ] = Grids::complex_type();
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ] = Grids::complex_type();
		
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ][ 0u ] = 0u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ][ 1u ] = 1u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ][ 2u ] = 2u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ][ 3u ] = 3u;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ][ 0u ] = 4u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ][ 1u ] = 5u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ][ 2u ] = 6u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ][ 3u ] = 7u;
	}
	
	void Builder::buildBox( Device * d, GridsID new_id, float box_size, float * color )
	{
		d->world_hash[ new_id ][ "Vertices" ][ 0u ][ "x" ] = -box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 0u ][ "y" ] = -box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 0u ][ "z" ] = -box_size;
		
		d->world_hash[ new_id ][ "Vertices" ][ 1u ][ "x" ] = box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 1u ][ "y" ] = -box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 1u ][ "z" ] = -box_size;
		
		d->world_hash[ new_id ][ "Vertices" ][ 2u ][ "x" ] = box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 2u ][ "y" ] = box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 2u ][ "z" ] = -box_size;
		
		d->world_hash[ new_id ][ "Vertices" ][ 3u ][ "x" ] = -box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 3u ][ "y" ] = box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 3u ][ "z" ] = -box_size;
		
		d->world_hash[ new_id ][ "Vertices" ][ 4u ][ "x" ] = -box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 4u ][ "y" ] = -box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 4u ][ "z" ] = box_size;
		
		d->world_hash[ new_id ][ "Vertices" ][ 5u ][ "x" ] = box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 5u ][ "y" ] = -box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 5u ][ "z" ] = box_size;
		
		d->world_hash[ new_id ][ "Vertices" ][ 6u ][ "x" ] = box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 6u ][ "y" ] = box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 6u ][ "z" ] = box_size;
		
		d->world_hash[ new_id ][ "Vertices" ][ 7u ][ "x" ] = -box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 7u ][ "y" ] = box_size;
		d->world_hash[ new_id ][ "Vertices" ][ 7u ][ "z" ] = box_size;
		
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "r" ] = color[0];
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "g" ] = color[1];
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "b" ] = color[2];
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "a" ] = color[3];
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Color" ] = 0u;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ][ 0u ] = 0u; // Box top
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ][ 1u ] = 1u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ][ 2u ] = 2u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 0u ][ 3u ] = 3u;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ][ 0u ] = 4u; // Box bottom
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ][ 1u ] = 5u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ][ 2u ] = 6u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 1u ][ 3u ] = 7u;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 2u ][ 0u ] = 4u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 2u ][ 1u ] = 5u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 2u ][ 2u ] = 1u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 2u ][ 3u ] = 0u;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 3u ][ 0u ] = 7u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 3u ][ 1u ] = 6u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 3u ][ 2u ] = 2u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 3u ][ 3u ] = 3u;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 4u ][ 0u ] = 4u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 4u ][ 1u ] = 7u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 4u ][ 2u ] = 3u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 4u ][ 3u ] = 0u;
		
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 5u ][ 0u ] = 5u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 5u ][ 1u ] = 6u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 5u ][ 2u ] = 2u;
		d->world_hash[ new_id ][ "Quads" ][ 0u ][ "Indices" ][ 5u ][ 3u ] = 1u;
	
	}
	
} // end namespace Kaleidoscope

