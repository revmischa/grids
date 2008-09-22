/*
 *  objectController.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <grids/objectController.h>
#include <kaleidoscope/simpleCube.h>


namespace Grids
{
	ObjectController::ObjectController( )
	{

	}
	
	ObjectController::~ObjectController( )
	{
		// go through all the pointers and delete the objects
		
	}
	
	

	void ObjectController::requestCreateObject( Kaleidoscope::Device * d, Value obj_value )
	{
		obj_value[ "_method" ] = "Room.Object.Create";
		
		d->getInterface()->sendEventDebug( obj_value[ "_method" ].asString() , obj_value );
	}
	
	void ObjectController::createObject( Kaleidoscope::Device * d, Value obj_value )
	{
		// how do I know what pointer this ID belongs to?
		// do I create a new cube?
		// Yes
		
		std::cout << "ObjectController creating object" << std::endl;
		
		
		if( obj_value[ "Kaleidoscope" ][ "Object.Type" ][ "Class.Name" ] == "SimpleCube" )
		{
			
			Kaleidoscope::SimpleCube * new_cube = new Kaleidoscope::SimpleCube();
			
			// register this fucker with the object controller
			registerObject( obj_value[ "id" ].asString(), new_cube );
			
			new_cube->create( d, obj_value );
		}
		
	}
	
	void ObjectController::registerObject( GridsID in_id, Object * in_ptr )
	{
		object_ids.push_back( in_id );
		id_pointer_hash[ in_id ] = in_ptr;
		pointer_id_hash[ in_ptr ] = in_id;
		
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
		ray_target *= -282.0f;
		
		Vec3D temp_box_position = ray_pos + ray_target;
		
		d->world_hash[ "TINY_BOX0" ][ "position" ][ 0u ] = temp_box_position.X;
		d->world_hash[ "TINY_BOX0" ][ "position" ][ 1u ] = temp_box_position.Y;
		d->world_hash[ "TINY_BOX0" ][ "position" ][ 2u ] = temp_box_position.Z;
		
//		std::cout << "Target   " << screen_ray[ 3 ] << " : " << screen_ray[ 4 ] << " : " << screen_ray[ 5 ] << std::endl;
//		std::cout << "Box pos  " << temp_box_position.X << " : " << temp_box_position.Y << " : " << temp_box_position.Z << std::endl;
//		std::cout << "Position  " << d->Position.X << " : " << d->Position.Y << " : " << d->Position.Z << std::endl;
//		std::cout << "Target  " << d->Target.X << " : " << d->Target.Y << " : " << d->Target.Z << std::endl;
//		std::cout << "Up  " << d->UpVector.X << " : " << d->UpVector.Y << " : " << d->UpVector.Z << std::endl;

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
	
	void ObjectController::requestUpdateValue( Kaleidoscope::Device * d, Object * obj_ptr, Value in_value )
	{
		GridsID temp_id = pointer_id_hash[ obj_ptr ];
		
		if(  temp_id.size() != 0  )
		{
			requestUpdateValue(d, temp_id, in_value );
		}
		
	}
	
	void ObjectController::requestUpdateValue( Kaleidoscope::Device * d, GridsID obj_id, Value in_value )
	{
		// assemble a correct event, with position, scale, and rotation properly filled in
		// send the request to the interface
		
		Value temp_value = Value();
		temp_value[ "id" ] = obj_id;
		
		// Need details for what the position, etc will be
		
		d->getInterface()->sendEvent( "Room.Object.Update", temp_value );
		
	}
	
	void ObjectController::updateValue( Kaleidoscope::Device * d, Object * obj_ptr, Value obj_value)
	{
		GridsID temp_id = pointer_id_hash[ obj_ptr ];
		
		if(  temp_id.size() != 0  )
		{
			updateValue(d, temp_id, obj_value );
		}
		
	}
	
	void ObjectController::updateValue( Kaleidoscope::Device * d, GridsID obj_id, Value obj_value)
	{
		if( !( d->world_hash[ obj_id ] ) == false ) // if the object hash position has been created
		{
			Vec3D obj_pos;
			Vec3D obj_rot;
			Vec3D obj_scl;
			
			d->world_hash[ obj_id ][ "position" ][ 0u ] = obj_pos.X;
			d->world_hash[ obj_id ][ "position" ][ 1u ] = obj_pos.Y;
			d->world_hash[ obj_id ][ "position" ][ 2u ] = obj_pos.Z;
			
			d->world_hash[ obj_id ][ "rotation" ][ 0u ] = obj_rot.X;
			d->world_hash[ obj_id ][ "rotation" ][ 1u ] = obj_rot.Y;
			d->world_hash[ obj_id ][ "rotation" ][ 2u ] = obj_rot.Z;
			
			d->world_hash[ obj_id ][ "scale" ][ 0u ] = obj_scl.X;
			d->world_hash[ obj_id ][ "scale" ][ 1u ] = obj_scl.Y;
			d->world_hash[ obj_id ][ "scale" ][ 2u ] = obj_scl.Z;
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

