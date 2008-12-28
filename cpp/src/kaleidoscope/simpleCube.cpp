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
#include <sstream>

namespace Kaleidoscope
{
	
	SimpleCube::SimpleCube( ) // Device * d, GridsID cube_room, Vec3D cube_position, float side_length, float * cube_color )
	{
		// Notify grids that we're making an object, etc.
		// requestCreateCube( d, cube_room, cube_position, side_length, cube_color );
		
	}
	
	
	void SimpleCube::requestCreateCube( Device * d, GridsID cube_room, Vec3D cube_position, float side_length, float * cube_color )
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
		
		(*temp_value)[ "pos" ][ 0u ] = cube_position.X; // Position
		(*temp_value)[ "pos" ][ 1u ] = cube_position.Y;
		(*temp_value)[ "pos" ][ 2u ] = cube_position.Z;
		
		(*temp_value)[ "rot" ][ 0u ] = 0.0f; // Rotation
		(*temp_value)[ "rot" ][ 1u ] = 0.0f;
		(*temp_value)[ "rot" ][ 2u ] = 0.0f;
		
		(*temp_value)[ "scl" ][ 0u ] = 1.0f; // Scale
		(*temp_value)[ "scl" ][ 1u ] = 1.0f;
		(*temp_value)[ "scl" ][ 2u ] = 1.0f;
		
		d->getInterface()->getObjectController()->requestCreateObject(d, temp_value );
	}
	
	void SimpleCube::create( Device * d, Grids::Value * in_value )
	{		
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
		
		return distanceToSphereAlongRay( ray_position, ray_target, this_position, d->world_hash[ this_id ][ "radius" ].asDouble() );// ray_pos, ray_tar, obj_pos, obj_radius )
		
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

