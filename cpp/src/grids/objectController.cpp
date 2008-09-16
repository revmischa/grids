/*
 *  objectController.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <grids/objectController.h>


namespace Grids
{
	ObjectController::ObjectController( )
	{

	}
	

	void ObjectController::requestCreateObject( Kaleidoscope::Device * d )
	{
		Value temp_value = Value();
		
		temp_value[ "_method" ] = "Room.Create" ;
		
		d->getInterface()->sendEvent( "Room.Create", temp_value );
	}
	
	void ObjectController::requestCreateObject( Kaleidoscope::Device * d, GridsID room_id )
	{
		std::string method = "Room.Object.Create";
		
		Value temp_value = Value();
		
		temp_value[ "_method" ] = method;
		
		d->getInterface()->sendEvent( method, temp_value );
	}
	
	void ObjectController::createObject( Kaleidoscope::Device * d, GridsID obj_id )
	{
		createObject(d, obj_id, Vec3D( 0.0f, 0.0f, 0.0f), Vec3D( 1.0f, 1.0f, 1.0f ), Vec3D( 0.0f, 0.0f, 0.0f ) ); 
		
	}
	
	void ObjectController::createObject( Kaleidoscope::Device * d, GridsID obj_id, GridsID room_id  )
	{
		createObject(d, obj_id, room_id, Vec3D( 0.0f, 0.0f, 0.0f), Vec3D( 1.0f, 1.0f, 1.0f ), Vec3D( 0.0f, 0.0f, 0.0f ) );
		
	}
	
	void ObjectController::createObject( Kaleidoscope::Device * d, GridsID obj_id, Vec3D in_pos, Vec3D in_rot, Vec3D in_scl )
	{
		 
		
		// umm, when do you actually make the object???
		
		addIdToVector( obj_id );
			
	}
	
	void ObjectController::createObject( Kaleidoscope::Device * d, GridsID obj_id, GridsID room_id, Vec3D in_pos, Vec3D in_rot, Vec3D in_scl  )
	{
		d->getBuilder()->placeObject( d, obj_id, room_id, in_pos, in_rot, in_scl );
		
		addIdToVector( obj_id );
		
	}
	
	void ObjectController::addIdToVector( GridsID obj_id )
	{
		bool found = false;
		
		for( int i = 0; i < object_ids.size(); i++ )
		{
			if( object_ids[ i ] == obj_id )
			{
				found = true;
			}
		}
		
		if( !found )
		{
			object_ids.push_back( obj_id );
		}
	}
		
	
	
	void ObjectController::detectSelection( Kaleidoscope::Device * d, int mouse_x, int mouse_y )
	{
		GridsID temp_id;
		GridsID closest_id;
		float closest_dist = 1000000.0f;  // FIX
		float temp_dist;
		
		// calculate the Ray from mouse position
		float screen_ray[ 6 ];
		
		d->getCamera()->getRayFromScreenCoordinates( d, Vec2D( mouse_x, mouse_y ), &screen_ray[ 0 ] );
		
		Vec3D ray_pos = Vec3D( screen_ray[ 0 ], screen_ray[ 1 ], screen_ray[ 2 ] );
		Vec3D ray_target = Vec3D( screen_ray[ 3 ], screen_ray[4 ], screen_ray[ 5 ] );
		
		for( int i = 0; i < object_ids.size(); i++ )
		{
			temp_id = object_ids[ i ];
			
			temp_dist = (id_pointer_hash[ temp_id ])->detectSelection( d, ray_pos, ray_target );
			
			if( temp_dist != -1 && temp_dist < closest_dist )
			{
				closest_id = temp_id;
			}
		}
		
		if( closest_id.size() != 0 )
		{
			(id_pointer_hash[ temp_id ])->selectObject( d );
		}
			
	}
	
	void ObjectController::requestUpdatePosition( Kaleidoscope::Device * d, Object * obj_ptr, Vec3D in_pos, Vec3D in_rot, Vec3D in_scl )
	{
		GridsID temp_id = pointer_id_hash[ obj_ptr ];
		
		if(  temp_id.size() != 0  )
		{
			requestUpdatePosition(d, temp_id, in_pos, in_rot, in_scl);
		}
		
	}
	
	void ObjectController::requestUpdatePosition( Kaleidoscope::Device * d, GridsID obj_id, Vec3D in_pos, Vec3D in_rot, Vec3D in_scl )
	{
		// assemble a correct event, with position, scale, and rotation properly filled in
		// send the request to the interface
		
		Value temp_value = Value();
		temp_value[ "id" ] = obj_id;
		
		// Need details for what the position, etc will be
		
		d->getInterface()->sendEvent( "Room.Object.Update", temp_value );
		
	}
	
	void ObjectController::updatePosition( Kaleidoscope::Device * d, Object * obj_ptr, Vec3D in_pos, Vec3D in_rot, Vec3D in_scl )
	{
		GridsID temp_id = pointer_id_hash[ obj_ptr ];
		
		if(  temp_id.size() != 0  )
		{
			updatePosition(d, temp_id, in_pos, in_rot, in_scl);
		}
		
	}
	
	void ObjectController::updatePosition( Kaleidoscope::Device * d, GridsID obj_id, Vec3D in_pos, Vec3D in_rot, Vec3D in_scl )
	{
		if( !( d->world_hash[ obj_id ] ) == false ) // if the object hash position has been created
		{
			d->world_hash[ obj_id ][ "position" ][ 0u ] = in_pos.X;
			d->world_hash[ obj_id ][ "position" ][ 1u ] = in_pos.Y;
			d->world_hash[ obj_id ][ "position" ][ 2u ] = in_pos.Z;
			
			d->world_hash[ obj_id ][ "rotation" ][ 0u ] = in_rot.X;
			d->world_hash[ obj_id ][ "rotation" ][ 1u ] = in_rot.Y;
			d->world_hash[ obj_id ][ "rotation" ][ 2u ] = in_rot.Z;
			
			d->world_hash[ obj_id ][ "scale" ][ 0u ] = in_scl.X;
			d->world_hash[ obj_id ][ "scale" ][ 1u ] = in_scl.Y;
			d->world_hash[ obj_id ][ "scale" ][ 2u ] = in_scl.Z;
		}
	}
	
	float ObjectController::intersectRaySphere(Vec3D rayPos, Vec3D rayDir, Vec3D sphereOrigin, float sphereRadius) 
	{
		Vec3D q = sphereOrigin - rayPos ;
		float c = q.getLength();
		float v = q.dotProduct(rayDir);
		float d = sphereRadius * sphereRadius - (c * c - v * v);
		
		// If there was no intersection, return -1
		if (d < 0.0)
		{
			return -1;
		}
		
		// Return the distance to the [first] intersecting point
		return v - (float)sqrt(d);
	} 
	
	GridsID ObjectController::getIdFromPointer( Object * obj_id )
	{
		GridsID temp_id = pointer_id_hash[ obj_id ];
		
		if( temp_id.size() != 0 )
		{
			return temp_id;
		}
		else
		{
			return GRIDS_ID_ERROR;
		}
	}


	
} // end namespace Grids

