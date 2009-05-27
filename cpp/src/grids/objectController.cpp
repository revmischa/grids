/*
 *  objectController.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
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

#include <grids/objectController.h>
#include <grids/define.h>

#include <kaleidoscope/simpleCube.h>
#include <kaleidoscope/define.h>



namespace Grids
{
	ObjectController::ObjectController( )
	{

	}
	
	ObjectController::~ObjectController( )
	{
		// go through all the pointers and delete the objects
		
	}
	

	void ObjectController::requestCreateObject( Kaleidoscope::Device * d, Value * obj_value )
	{		
		Value * temp_type = new Value();
		
		(*temp_type)[ "_method" ] = GRIDS_CREATE_OBJECT;
		(*temp_type)[ "room_id" ] = (*obj_value)[ "room_id" ].asString();
		(*temp_type)[ "attr" ] = (*obj_value);
				
		d->getInterface()->sendEvent( GRIDS_CREATE_OBJECT, temp_type );
		
		delete temp_type;
	}
	
	void ObjectController::createObject( Kaleidoscope::Device * d, Value * obj_value )
	{
		std::cout << "ObjectController Creating object <  " << (*obj_value)[ "req" ][ "attr" ][ "type" ][ "name" ].asString() << " >" << std::endl;
		
		//std::string temp_string = "SimpleCube";
		
		d->temp_box_id = (*obj_value)[ "id" ].asString();
		
		if( (*obj_value)[ "req" ][ "attr" ][ "type" ][ "name" ].asString() == "SimpleCube" )
		{			
			Kaleidoscope::SimpleCube * new_cube = new Kaleidoscope::SimpleCube();
			
			// register this with the object controller
			registerObject( (*obj_value)[ "id" ].asString(), new_cube );
			
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
		float closest_dist = d->Far; // You shouldn't be able to select objects farther than you can see.
		float temp_dist;
		
		// calculate the Ray from mouse position
		float screen_ray[ 6 ];
		
		d->getCamera()->getRayFromScreenCoordinates( d, Vec2D( mouse_x, mouse_y ), &screen_ray[ 0 ] );
		
//		std::cout << screen_ray[ 0 ] << " : " << screen_ray[ 1 ] << " : " << screen_ray[ 2 ] << " : " 
//			<< screen_ray[ 3 ] << " : " << screen_ray[ 4 ] << " : " << screen_ray[ 5 ] << std::endl;
		
		Vec3D ray_pos = Vec3D( screen_ray[ 0 ], screen_ray[ 1 ], screen_ray[ 2 ] );
		Vec3D ray_target = Vec3D( screen_ray[ 3 ], screen_ray[4 ], screen_ray[ 5 ] );
				
		for( int i = 0; i < object_ids.size(); i++ )
		{
			temp_id = object_ids[ i ];
			
			temp_dist = getDistFromRay( d, temp_id, ray_pos, ray_target );
			
			if( temp_dist != -1 && temp_dist < closest_dist )
			{
				closest_id = temp_id;
			}
		}
		
		if( closest_id.size() != 0 )
		{
			selectObject( d, closest_id );
		}
			
	}
	
	
	float ObjectController::getDistFromRay( Kaleidoscope::Device * d, GridsID in_id, Vec3D ray_pos, Vec3D ray_target )
	{
		d->lockWorldHash();
		std::string object_name = d->world_hash[ in_id ][ "type" ][ "name" ].asString();
		d->unlockWorldHash();
		
		if( object_name == "SimpleCube" )
		{
			return ( (Kaleidoscope::SimpleCube *) id_pointer_hash[ in_id ])->detectSelection( d, ray_pos, ray_target );
		}
		
		return -1.0f;
	}
	
	void ObjectController::selectObject( Kaleidoscope::Device * d, GridsID in_id )
	{
		d->lockWorldHash();
		std::string object_name = d->world_hash[ in_id ][ "type" ][ "name" ].asString();
		d->unlockWorldHash();

		if( object_name == "SimpleCube" )
		{
			( (Kaleidoscope::SimpleCube *) id_pointer_hash[ in_id ])->selectObject( d );
		}
	}
	
	void ObjectController::parseUpdate( Kaleidoscope::Device * d, Value * in_value )
	{
		if( (*in_value)[ "req" ][ "attr" ][ UPDATE_TYPE ].asString() == UPDATE_POSITION )
		{
			updatePosition( d, in_value );
		}
	}
	
	
	void ObjectController::requestUpdatePasition( Kaleidoscope::Device * d, GridsID obj_id, GridsID room_id, Vec3D new_position )
	{
		Value * temp_type = new Value();
		Value * attr_value = new Value();
		
		( *attr_value )[ "pos" ][ 0u ] = new_position.X;
		( *attr_value )[ "pos" ][ 1u ] = new_position.Y;
		( *attr_value )[ "pos" ][ 2u ] = new_position.Z;
		
		( *attr_value )[ UPDATE_TYPE ] = UPDATE_POSITION;
		
		( *attr_value )[ "room_id" ] = room_id;
		
		(*temp_type)[ "_method" ] = GRIDS_UPDATE_OBJECT;
		(*temp_type)[ "room_id" ] = room_id;
		(*temp_type)[ "id" ] = obj_id;
		(*temp_type)[ "attr" ] = (*attr_value);
		
		d->getInterface()->sendEvent( GRIDS_UPDATE_OBJECT, temp_type );
		
		delete temp_type;
		delete attr_value;
	}
	
	void ObjectController::updatePosition( Kaleidoscope::Device * d, Value * in_value )
	{
		GridsID object_id = (*in_value)[ "id" ].asString();
		float temp_x = (*in_value)[ "req" ][ "attr" ][ "pos" ][ 0u ].asDouble();
		float temp_y = (*in_value)[ "req" ][ "attr" ][ "pos" ][ 1u ].asDouble();
		float temp_z = (*in_value)[ "req" ][ "attr" ][ "pos" ][ 2u ].asDouble();
		
		d->world_hash[ object_id ][ "position" ][ 0u ] = temp_x;
		d->world_hash[ object_id ][ "position" ][ 1u ] = temp_y;
		d->world_hash[ object_id ][ "position" ][ 2u ] = temp_z;
	}
	
	
	void ObjectController::requestUpdateValue( Kaleidoscope::Device * d, Object * obj_ptr, Value * in_value )
	{
		GridsID temp_id = pointer_id_hash[ obj_ptr ];
		
		if(  temp_id.size() != 0  )
		{
			requestUpdateValue(d, temp_id, in_value );
		}
		
	}
	
	void ObjectController::requestUpdateValue( Kaleidoscope::Device * d, GridsID obj_id, Value * in_value )
	{
		// assemble a correct event, with position, scale, and rotation properly filled in
		// send the request to the interface
		
		(*in_value)[ "id" ] = obj_id;
		
		// Need details for what the position, etc will be
		
		d->getInterface()->sendEvent( GRIDS_UPDATE_OBJECT, in_value );
		
	}
	
	void ObjectController::updateValue( Kaleidoscope::Device * d, Object * obj_ptr, Value obj_value)
	{
		GridsID temp_id = pointer_id_hash[ obj_ptr ];
		
		if(  temp_id.size() != 0  )
		{
			updateValue(d, temp_id, &obj_value );
		}
	}	
	
	void ObjectController::updateValue( Kaleidoscope::Device * d, GridsID obj_id, Value obj_value)
	{
		updateValue(d, obj_id, &obj_value);
	}
	
	void ObjectController::updateValue( Kaleidoscope::Device * d, GridsID obj_id, Value * obj_value )
	{
		d->lockWorldHash();
		bool object_doesnt_exist = !( d->world_hash[ obj_id ] );
		d->unlockWorldHash();
		
		if( object_doesnt_exist == false ) // if the object hash position has been created
		{
			Vec3D obj_pos = Vec3D( (*obj_value)[ "position" ][ 0u ].asDouble(), 
								  (*obj_value)[ "position" ][ 1u ].asDouble(), 
								  (*obj_value)[ "position" ][ 2u ].asDouble() ) ;
			Vec3D obj_rot = Vec3D( (*obj_value)[ "rotation" ][ 0u ].asDouble(), 
								  (*obj_value)[ "rotation" ][ 1u ].asDouble(), 
								  (*obj_value)[ "rotation" ][ 2u ].asDouble()	) ;
			Vec3D obj_scl = Vec3D( (*obj_value)[ "scale" ][ 0u ].asDouble(), 
								  (*obj_value)[ "scale" ][ 1u ].asDouble(), 
								  (*obj_value)[ "scale" ][ 2u ].asDouble()	);
			
			d->lockWorldHash();
			
			d->world_hash[ obj_id ][ "position" ][ 0u ] = obj_pos.X;
			d->world_hash[ obj_id ][ "position" ][ 1u ] = obj_pos.Y;
			d->world_hash[ obj_id ][ "position" ][ 2u ] = obj_pos.Z;
			
			d->world_hash[ obj_id ][ "rotation" ][ 0u ] = obj_rot.X;
			d->world_hash[ obj_id ][ "rotation" ][ 1u ] = obj_rot.Y;
			d->world_hash[ obj_id ][ "rotation" ][ 2u ] = obj_rot.Z;
			
			d->world_hash[ obj_id ][ "scale" ][ 0u ] = obj_scl.X;
			d->world_hash[ obj_id ][ "scale" ][ 1u ] = obj_scl.Y;
			d->world_hash[ obj_id ][ "scale" ][ 2u ] = obj_scl.Z;
			
			d->unlockWorldHash();
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

