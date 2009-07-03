/*
 *  object.cpp
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

#include <grids/object.h>

#include <grids/event.h>
#include <grids/objectController.h>

#include <kaleidoscope/device.h>
#include <kaleidoscope/geo.h>


namespace Grids
{
	Object::Object( Kal::Device* d, Value* in_val ){		
		Kal::Utility::puts( "new Object" );

		GridsID temp_id = getIDFromValue( in_val );
		
		Kal::Utility::puts( temp_id );
		setID( temp_id );
		
		d->getInterface()->getObjectController()->registerObject( temp_id, this );
	}

	void Object::requestUpdatePosition( Kaleidoscope::Device * d, Vec3D in_pos, Vec3D in_rot, Vec3D in_scl )
	// Called when an event is received
	{
		Value * temp_value = new Value();
		// generate actual value
		
		loadPosition( temp_value, in_pos, in_rot, in_scl );
		
		d->getInterface()->getObjectController()->requestUpdateValue(d, this,  temp_value );
		
		delete temp_value;
	}

	void Object::updatePosition( Kaleidoscope::Device * d, Value * in_value )
	{
		// generate actual value
		
		//d->getInterface()->getObjectController()->updateValue( d, this, in_value );
	}
	
	//fvoid Object::create( Kaleidoscope::Device * d, Value in_value )
	

	float Object::detectSelection( Kaleidoscope::Device * d, GridsID object_id, Vec3D ray_position, Vec3D ray_target )
	{
		//Grids::GridsID this_id = d->getInterface()->getObjectController()->getIdFromPointer( this );
		GridsID this_id = object_id;
		
		if( this_id == GRIDS_ID_ERROR )
		{
			return -1;
		}  
		
		Vec3D this_position = Vec3D( d->world_hash[ this_id ][ "position" ][ 0u ].asDouble(),
							    d->world_hash[ this_id ][ "position" ][ 1u ].asDouble(),
							    d->world_hash[ this_id ][ "position" ][ 2u ].asDouble()	);
		
		if( !( d->world_hash[ this_id ][ "radius" ] ) ) // if there is no "radius" entry
		{
			// Get the object's pointer, and 
			Grids::Object* temp_object = (Grids::Object*)(d->getInterface()->getObjectController()->getIdPointerHash())[ object_id ]; 
			temp_object->calculateRadius( d, this_id );		
		}
		
		return Kaleidoscope::Geo::distanceToSphereAlongRay( ray_position, ray_target, this_position, d->world_hash[ this_id ][ "radius" ].asDouble() );// ray_pos, ray_tar, obj_pos, obj_radius )
		
		// detect intersection with radius
	}

	
	void Object::selectObject( Kaleidoscope::Device * d )
	{
		std::cerr << "Object method should exist in parent object" << std::endl;     
		
	}

	void Object::setController( ObjectController * cont )
	{
		controller = cont ;
	}
	
	float Object::calculateRadius( Kaleidoscope::Device * d, GridsID this_id ){
		return calculateRadiusFromVertices( d, this_id );
	}
	
	float Object::calculateRadiusFromVertices( Kaleidoscope::Device * d, GridsID this_id )
	{
		// calculate the radius by looking at all vertices and the average scale
		// and store that in the "radius" slot
		
		float radius = 0.0f;
		Vec3D temp_rad_vec;
		
		for( int i = 0; i < d->world_hash[ this_id ][ "vertices" ].size(); i++ )
			{
				temp_rad_vec = Vec3D(  d->world_hash[ this_id ][ "vertices" ][ i ][ 0u ].asDouble(),
								   d->world_hash[ this_id ][ "vertices" ][ i ][ 1u ].asDouble(),
								   d->world_hash[ this_id ][ "vertices" ][ i ][ 2u ].asDouble()	);
			
			if( temp_rad_vec.getLength() > radius )
			{
				radius = temp_rad_vec.getLength();
			}
		}
		
		d->world_hash[ this_id ][ "radius" ] = radius;
		
		return radius;
	}
	
	void Object::loadPosition( Value * temp_value, Vec3D pos, Vec3D rot, Vec3D scl ){
		(*temp_value)[ "pos" ][ 0u ] = pos.X; // Position
		(*temp_value)[ "pos" ][ 1u ] = pos.Y;
		(*temp_value)[ "pos" ][ 2u ] = pos.Z;
		
		(*temp_value)[ "rot" ][ 0u ] = rot.X; // Rotation
		(*temp_value)[ "rot" ][ 1u ] = rot.Y;
		(*temp_value)[ "rot" ][ 2u ] = rot.Z;
		
		(*temp_value)[ "scl" ][ 0u ] = scl.X; // Scale
		(*temp_value)[ "scl" ][ 1u ] = scl.Y;
		(*temp_value)[ "scl" ][ 2u ] = scl.Z;
	}

	void Object::setRoom( Value* temp_value, GridsID in_room ){
		(*temp_value)[ "room_id" ] = in_room;
	}
	
	GridsID Object::getID( Kal::Device* d ){		
		return d->getInterface()->getObjectController()->getIdFromPointer( this );
	}
	
	GridsID Object::getID( ){
		return this_id;
	}
	
	void Object::setID( GridsID new_id ){
		this_id = new_id;
	}
	
	GridsID Object::getRoomID(){
		return room_id;
	}

	void Object::setRoomID( GridsID new_room ){
		room_id = new_room;
	}
	
	GridsID Object::getIDFromValue( Value* in_value ){
		return (*in_value)[ "req" ][ "attr" ][ "id" ].asString();
	}
	
	Value* Object::getAttr( Value* in_value ){
		return new Value( (*in_value)[ "req" ][ "attr" ] );	
	}

	std::string Object::getNameFromValue( Value* in_val ){
		return (*in_val)[ "req" ][ "attr" ][ "type" ][ "name" ].asString();
	}




} // end namespace Grids

