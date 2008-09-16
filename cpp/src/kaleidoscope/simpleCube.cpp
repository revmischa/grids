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
	
	SimpleCube::SimpleCube( Device * d, GridsID cube_room, Vec3D cube_position, float * cube_color )
	{
		requestCreateCube( d, cube_room, cube_position, cube_color );
		
	}
	
	void SimpleCube::requestCreateCube( Device * d, GridsID cube_room, Vec3D cube_position, float * cube_color )
	{
		//d->getInterface()->getObjectController()->
		
	}
	
	void SimpleCube::create( Device * d, Grids::Value cube_value )
	{
		Grids::GridsID cube_id;
		Vec3D cube_position;
		Vec3D cube_rotation;
		Vec3D cube_scale;
		
		d->getBuilder()->placeObject( d, cube_id, cube_position, cube_rotation, cube_scale );
		
	}
	
	float SimpleCube::detectSelection( Device * d, Vec3D ray_position, Vec3D ray_target )
	{
		Grids::GridsID this_id = d->getInterface()->getObjectController()->getIdFromPointer( this );
		
		if( this_id == GRIDS_ID_ERROR )
		{
			return -1;
		}  
		
		if( !( d->world_hash[ this_id ][ "radius" ] ) )
		{
			// calculate the radius by looking at all vertices and the average scale
			// and store that in the "radius" slot
		}
		else
		{
			
		}
	}
	
	void SimpleCube::selectObject( Device * d )
	{
		
	}
	
} // end namespace Kaleidoscope

