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

#include <kaleidoscope/simpleCube.h>

#include <kaleidoscope/device.h>
#include <kaleidoscope/geo.h>

namespace Kaleidoscope
{
	
	SimpleCube::SimpleCube( Device* d, Grids::Value* in_val ) : GLSpaceObject( d, in_val ) 
	{
		// Notify grids that we're making an object, etc.
		// requestCreateCube( d, cube_room, cube_position, side_length, cube_color );
		
	}
	
	
	GridsID SimpleCube::requestCreate( Device * d, GridsID cube_room, Vec3D cube_position, float side_length, float * cube_color )
	{
		Grids::Value * temp_value = new Grids::Value();
		
		//(*temp_value)[ "_method" ] = GRIDS_CREATE_OBJECT;
		(*temp_value)[ "room_id" ] = cube_room;
		
		(*temp_value)[ "type" ][ "name" ] = "SimpleCube";
		
		// Start all the variables specific to this object type
		(*temp_value)[ "type" ][ "CubeColor" ][ 0u ] = cube_color[ 0u ];
		(*temp_value)[ "type" ][ "CubeColor" ][ 1u ] = cube_color[ 1u ];
		(*temp_value)[ "type" ][ "CubeColor" ][ 2u ] = cube_color[ 2u ];
		(*temp_value)[ "type" ][ "CubeColor" ][ 3u ] = cube_color[ 3u ];
		
		(*temp_value)[ "type" ][ "SideLength" ] = side_length;
		// End all the variables specific to this object type

		loadPosition( temp_value, cube_position, Vec3D( 0.0f, 0.0f, 0.0f), Vec3D( 1.0f, 1.0f, 1.0f) );
		
		loadGeometry( temp_value, side_length, cube_color );
		
		return d->getInterface()->getObjectController()->requestCreateObject(d, temp_value );
	}


	void SimpleCube::draw( Device* d ){

	}
	
	void SimpleCube::create( Device * d, Grids::Value * in_value )
	{		
	}

	void SimpleCube::loadGeometry(  Grids::Value* in_val, float box_size, float * color )
	{
		(*in_val)[ "vertices" ][ 0u ][ 0u ] = -box_size;
		(*in_val)[ "vertices" ][ 0u ][ 1u ] = -box_size;
		(*in_val)[ "vertices" ][ 0u ][ 2u ] = -box_size;

		(*in_val)[ "vertices" ][ 1u ][ 0u ] = box_size;
		(*in_val)[ "vertices" ][ 1u ][ 1u ] = -box_size;
		(*in_val)[ "vertices" ][ 1u ][ 2u ] = -box_size;

		(*in_val)[ "vertices" ][ 2u ][ 0u ] = box_size;
		(*in_val)[ "vertices" ][ 2u ][ 1u ] = box_size;
		(*in_val)[ "vertices" ][ 2u ][ 2u ] = -box_size;

		(*in_val)[ "vertices" ][ 3u ][ 0u ] = -box_size;
		(*in_val)[ "vertices" ][ 3u ][ 1u ] = box_size;
		(*in_val)[ "vertices" ][ 3u ][ 2u ] = -box_size;

		(*in_val)[ "vertices" ][ 4u ][ 0u ] = -box_size;
		(*in_val)[ "vertices" ][ 4u ][ 1u ] = -box_size;
		(*in_val)[ "vertices" ][ 4u ][ 2u ] = box_size;

		(*in_val)[ "vertices" ][ 5u ][ 0u ] = box_size;
		(*in_val)[ "vertices" ][ 5u ][ 1u ] = -box_size;
		(*in_val)[ "vertices" ][ 5u ][ 2u ] = box_size;

		(*in_val)[ "vertices" ][ 6u ][ 0u ] = box_size;
		(*in_val)[ "vertices" ][ 6u ][ 1u ] = box_size;
		(*in_val)[ "vertices" ][ 6u ][ 2u ] = box_size;

		(*in_val)[ "vertices" ][ 7u ][ 0u ] = -box_size;
		(*in_val)[ "vertices" ][ 7u ][ 1u ] = box_size;
		(*in_val)[ "vertices" ][ 7u ][ 2u ] = box_size;

		(*in_val)[ "color" ][ 0u ][ 0u ] = color[0];
		(*in_val)[ "color" ][ 0u ][ 1u ] = color[1];
		(*in_val)[ "color" ][ 0u ][ 2u ] = color[2];
		(*in_val)[ "color" ][ 0u ][ 3u ] = color[3];

		(*in_val)[ "quads" ][ 0u ][ "color" ] = 0u;

		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 0u ][ 0u ] = 0u; // Box top
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 0u ][ 1u ] = 1u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 0u ][ 2u ] = 2u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 0u ][ 3u ] = 3u;

		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 1u ][ 0u ] = 4u; // Box bottom
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 1u ][ 1u ] = 5u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 1u ][ 2u ] = 6u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 1u ][ 3u ] = 7u;

		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 2u ][ 0u ] = 4u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 2u ][ 1u ] = 5u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 2u ][ 2u ] = 1u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 2u ][ 3u ] = 0u;

		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 3u ][ 0u ] = 7u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 3u ][ 1u ] = 6u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 3u ][ 2u ] = 2u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 3u ][ 3u ] = 3u;

		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 4u ][ 0u ] = 4u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 4u ][ 1u ] = 7u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 4u ][ 2u ] = 3u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 4u ][ 3u ] = 0u;

		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 5u ][ 0u ] = 5u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 5u ][ 1u ] = 6u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 5u ][ 2u ] = 2u;
		(*in_val)[ "quads" ][ 0u ][ "indices" ][ 5u ][ 3u ] = 1u;
		
		
		// Line Color
		(*in_val)[ "color" ][ 1u ][ 0u ] = 0.75f;
		(*in_val)[ "color" ][ 1u ][ 1u ] = 0.75f;
		(*in_val)[ "color" ][ 1u ][ 2u ] = 0.75f;
		(*in_val)[ "color" ][ 1u ][ 3u ] = 0.15f;
		
		(*in_val)[ "lines" ][ 0u ][ "color" ] = 1u;
		
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 0u ][ 0u ] = 0u;
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 0u ][ 1u ] = 1u;
		
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 1u ][ 0u ] = 1u;
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 1u ][ 1u ] = 2u;
		
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 2u ][ 0u ] = 2u;
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 2u ][ 1u ] = 3u;
		
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 3u ][ 0u ] = 3u;
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 3u ][ 1u ] = 0u;
		
		
		
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 4u ][ 0u ] = 4u;
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 4u ][ 1u ] = 5u;
		
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 5u ][ 0u ] = 5u;
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 5u ][ 1u ] = 6u;
		
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 6u ][ 0u ] = 6u;
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 6u ][ 1u ] = 7u;
		
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 7u ][ 0u ] = 7u;
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 7u ][ 1u ] = 4u;
		
		
		
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 8u ][ 0u ] = 0u;
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 8u ][ 1u ] = 4u;
		
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 9u ][ 0u ] = 1u;
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 9u ][ 1u ] = 5u;
		
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 10u ][ 0u ] = 2u;
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 10u ][ 1u ] = 6u;
		
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 11u ][ 0u ] = 3u;
		(*in_val)[ "lines" ][ 0u ][ "indices" ][ 11u ][ 1u ] = 7u;
	}

	
	float SimpleCube::detectSelection( Device * d, Vec3D ray_position, Vec3D ray_target )
	{
		Grids::GridsID this_id = d->getInterface()->getObjectController()->getIdFromPointer( this );
		
		if( this_id == GRIDS_ID_ERROR )
		{
			return -1;
		}  
		
		Vec3D this_position = Vec3D( d->world_hash[ this_id ][ "position" ][ 0u ].asDouble(),
									d->world_hash[ this_id ][ "position" ][ 1u ].asDouble(),
									d->world_hash[ this_id ][ "position" ][ 2u ].asDouble()	);
		
		if( !( d->world_hash[ this_id ][ "radius" ] ) ) // if there is no "radius" entry
		{
			calculateRadiusFromVertices( d, this_id );
		}
		
		return Geo::distanceToSphereAlongRay( ray_position, ray_target, this_position, d->world_hash[ this_id ][ "radius" ].asDouble() );// ray_pos, ray_tar, obj_pos, obj_radius )
		
		// detect intersection with radius
	}
	
	void SimpleCube::selectObject( Device * d )
	// The object controller will call this.  (1) EventController recognises click (2) ObjectController checks selection with all objects
	// (3) If there is a selection, then call selectObject on the relevant object
	{
		Grids::GridsID this_id = d->getInterface()->getObjectController()->getIdFromPointer( this );
		
		d->world_hash[ this_id ][ "scale" ][ 0u ] = 2.0f;
		d->world_hash[ this_id ][ "scale" ][ 1u ] = 2.0f;
		d->world_hash[ this_id ][ "scale" ][ 2u ] = 2.0f;
		
		std::cout << "selected object -- " << this << std::endl;
		
	}



	
} // end namespace Kaleidoscope

/*
		// **********************************
		// THIS IS HOW IT SHOULD BE DONE!!!
		// **********************************
		//
		//Grids::Value * return_value =  ( *in_value )[ "req" ] ;
		//Grids::Value * attributes = ( *return_value )[ "attr" ];
		
		Grids::GridsID cube_id = (*in_value)[ "id" ].asString();
		Grids::GridsID room_id = (*in_value)[ "req" ][ "room_id" ].asString();
		
		std::cout << "Creating SimpleCube with id:  " << cube_id << std::endl;
		std::cout << "Creating SimpleCube in room:  " << room_id << std::endl;
		
		d->world_hash[ cube_id ][ "type" ][ "name" ] = "SimpleCube";
		
		//std::cout << cube_id << "    "  << room_id << std::endl;
		
		Vec3D cube_position = Vec3D( (*in_value)[ "req" ][ "attr" ][ "pos" ][ 0u ].asDouble(), 
									(*in_value)[ "req" ][ "attr" ][ "pos" ][ 1u ].asDouble(), 
									(*in_value)[ "req" ][ "attr" ][ "pos" ][ 2u ].asDouble()	);
		
		Vec3D cube_rotation = Vec3D( (*in_value)[ "req" ][ "attr" ][ "rot" ][ 0u ].asDouble(), 
									(*in_value)[ "req" ][ "attr" ][ "rot" ][ 1u ].asDouble(), 
									(*in_value)[ "req" ][ "attr" ][ "rot" ][ 2u ].asDouble()	);
		
		Vec3D cube_scale = Vec3D( (*in_value)[ "req" ][ "attr" ][ "scl" ][ 0u ].asDouble(), 
								 (*in_value)[ "req" ][ "attr" ][ "scl" ][ 1u ].asDouble(), 
								(*in_value)[ "req" ][ "attr" ][ "scl" ][ 2u ].asDouble()	);
	
		std::cout << "Pos:  " << cube_position.X << " : " << cube_position.Y << " : " << cube_position.Z << std::endl;
		std::cout << "Scale:  " << cube_scale.X << " : " << cube_scale.Y << " : " << cube_scale.Z << std::endl;
		std::cout << "Rot:  " << cube_rotation.X << " : " << cube_rotation.Y << " : " << cube_rotation.Z << std::endl;
		
		d->getBuilder()->placeObject( d, cube_id, room_id, cube_position, cube_scale, cube_rotation );
		
		float cube_color[4];
		cube_color[ 0 ] = (*in_value)[ "req" ][ "attr" ][ "type" ][ "CubeColor" ][ 0u ].asDouble();
		cube_color[ 1 ] = (*in_value)[ "req" ][ "attr" ][ "type" ][ "CubeColor" ][ 1u ].asDouble();
		cube_color[ 2 ] = (*in_value)[ "req" ][ "attr" ][ "type" ][ "CubeColor" ][ 2u ].asDouble();
		cube_color[ 3 ] = (*in_value)[ "req" ][ "attr" ][ "type" ][ "CubeColor" ][ 3u ].asDouble();
						
//		std::cout << "*******" << std::endl;
//		std::cout << d << std::endl;
//		std::cout << cube_id << std::endl;
//		std::cout << (*in_value)[ "Kaleidoscope" ][ "Object.Type" ][ "SideLength" ].asDouble() << std::endl;
//		std::cout << cube_color[0] << " : " << cube_color[1] << " : " << cube_color[2] << " : " << cube_color[3] << std::endl;
//		std::cout << "*******" << std::endl;

		d->getBuilder()->buildBox( d, cube_id, (*in_value)[ "req" ][ "attr" ][ "type" ][ "SideLength" ].asDouble(), &cube_color[0] );


 */
