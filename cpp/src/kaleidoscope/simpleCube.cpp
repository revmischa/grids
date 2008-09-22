/*
 *  simpleCube.cpp
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 9/15/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
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
		//d->getInterface()->getObjectController()->
		Grids::Value temp_value;
		
		temp_value[ "_method" ] = "Room.Object.Create";
		temp_value[ "Room.Id" ] = cube_room;
		
		temp_value[ "Kaleidoscope" ][ "Object.Type" ][ "Class.Name" ] = "SimpleCube";
		
		// Start all the variables specific to this object type
		temp_value[ "Kaleidoscope" ][ "Object.Type" ][ "CubeColor" ][ 0u ] = cube_color[ 0u ];
		temp_value[ "Kaleidoscope" ][ "Object.Type" ][ "CubeColor" ][ 1u ] = cube_color[ 1u ];
		temp_value[ "Kaleidoscope" ][ "Object.Type" ][ "CubeColor" ][ 2u ] = cube_color[ 2u ];
		temp_value[ "Kaleidoscope" ][ "Object.Type" ][ "CubeColor" ][ 3u ] = cube_color[ 3u ];
		
		temp_value[ "Kaleidoscope" ][ "Object.Type" ][ "SideLength" ] = side_length;
		// End all the variables specific to this object type
		
		temp_value[ "Kaleidoscope" ][ "Object.Position" ][ 0u ] = cube_position.X; // Position
		temp_value[ "Kaleidoscope" ][ "Object.Position" ][ 1u ] = cube_position.Y;
		temp_value[ "Kaleidoscope" ][ "Object.Position" ][ 2u ] = cube_position.Z;
		
		temp_value[ "Kaleidoscope" ][ "Object.Rotation" ][ 0u ] = 0.0f; // Rotation
		temp_value[ "Kaleidoscope" ][ "Object.Rotation" ][ 1u ] = 0.0f;
		temp_value[ "Kaleidoscope" ][ "Object.Rotation" ][ 2u ] = 0.0f;
		
		temp_value[ "Kaleidoscope" ][ "Object.Scale" ][ 0u ] = 1.0f; // Scale
		temp_value[ "Kaleidoscope" ][ "Object.Scale" ][ 1u ] = 1.0f;
		temp_value[ "Kaleidoscope" ][ "Object.Scale" ][ 2u ] = 1.0f;
		
		d->getInterface()->getObjectController()->requestCreateObject(d, temp_value );
	}
	
	void SimpleCube::create( Device * d, Grids::Value in_value )
	{		
		Grids::GridsID cube_id = in_value[ "id" ].asString();
		Grids::GridsID room_id = in_value[ "Room.Id" ].asString();
		
		std::cout << cube_id << "    "  << room_id << std::endl;
		
		Vec3D cube_position = Vec3D( in_value[ "Kaleidoscope" ][ "Object.Position" ][ 0u ].asDouble(), 
									in_value[ "Kaleidoscope" ][ "Object.Position" ][ 1u ].asDouble(), 
									in_value[ "Kaleidoscope" ][ "Object.Position" ][ 2u ].asDouble()	);
		
		Vec3D cube_rotation = Vec3D( in_value[ "Kaleidoscope" ][ "Object.Rotation" ][ 0u ].asDouble(), 
									in_value[ "Kaleidoscope" ][ "Object.Rotation" ][ 1u ].asDouble(), 
									in_value[ "Kaleidoscope" ][ "Object.Rotation" ][ 2u ].asDouble()	);
		
		Vec3D cube_scale = Vec3D( in_value[ "Kaleidoscope" ][ "Object.Scale" ][ 0u ].asDouble(), 
								 in_value[ "Kaleidoscope" ][ "Object.Scale" ][ 1u ].asDouble(), 
								in_value[ "Kaleidoscope" ][ "Object.Scale" ][ 2u ].asDouble()	);
		
		std::cout << "Pos:  " << cube_position.X << " : " << cube_position.Y << " : " << cube_position.Z << std::endl;
		std::cout << "Scale:  " << cube_scale.X << " : " << cube_scale.Y << " : " << cube_scale.Z << std::endl;
		std::cout << "Rot:  " << cube_rotation.X << " : " << cube_rotation.Y << " : " << cube_rotation.Z << std::endl;
		
		d->getBuilder()->placeObject( d, cube_id, room_id, cube_position, cube_rotation, cube_scale );
		
		float cube_color[4];
		cube_color[ 0 ] = in_value[ "Kaleidoscope" ][ "Object.Type" ][ "CubeColor" ][ 0u ].asDouble();
		cube_color[ 1 ] = in_value[ "Kaleidoscope" ][ "Object.Type" ][ "CubeColor" ][ 1u ].asDouble();
		cube_color[ 2 ] = in_value[ "Kaleidoscope" ][ "Object.Type" ][ "CubeColor" ][ 2u ].asDouble();
		cube_color[ 3 ] = in_value[ "Kaleidoscope" ][ "Object.Type" ][ "CubeColor" ][ 3u ].asDouble();
				
		//std::cout << "side:  " << in_value[ "Kaleidoscope" ][ "Object.Type" ][ "SideLength" ].asDouble() << std::endl;

		d->getBuilder()->buildBox(d, cube_id, in_value[ "Kaleidoscope" ][ "Object.Type" ][ "SideLength" ].asDouble(), &cube_color[0] );
						
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
		
	}
	
} // end namespace Kaleidoscope

