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

#include <kaleidoscope/room.h>

#include <grids/event.h>
#include <grids/objectController.h>

#include <kaleidoscope/device.h>
#include <kaleidoscope/geo.h>


namespace Grids
{
	Object::Object( Kal::Device* d, Value* in_val ){		
		Kal::Utility::puts( "new Object" );

		initMutex();

		Object* parent_ptr = getParentFromValue( d, in_val );

		Kal::Utility::puts( "Object::Object Parent: ", (unsigned int)parent_ptr );
		
		setParent( parent_ptr );
		
		if( parent_ptr )
			parent_ptr->addChild( this );		

		GridsID temp_id = getIDFromValue( in_val );
		
		Kal::Utility::puts( "New Object id = ", temp_id );
		setID( temp_id );
		
		d->getInterface()->getObjectController()->registerObject( temp_id, this );


		Value* temp_attr = getAttr( in_val );		
		attr = Value( *temp_attr );
		delete temp_attr;
				
		setID( attr[ "id" ].asString() );
		setRoomID( attr[ "room_id" ].asString() );				
	}

	Object::~Object(){
		deleteMutex();
		
		// All children should be deleted when the ObjectController goes through the id_pointer_hash and deletes 
		// all objects stored in there
		// In other words, ALL OBJECTS MUST BE REGISTERED
		// to prevent memory leaks!
		//
		//deleteChildren();
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
		
		Vec3D this_position = getPosition();
		
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
		Kal::Utility::puts( "Object getIDFromValue" );
		
		if( (*in_value)[ "_method" ].asString().compare( "Room.Create" ) == 0 )
			return (*in_value)[ "id" ].asString();
		else
			return (*in_value)[ "req" ][ "attr" ][ "id" ].asString();
	}
	
	Value* Object::getAttr( Value* in_value ){
		return new Value( (*in_value)[ "req" ][ "attr" ] );	
	}
	
	Value* Object::getAttr(){
		return &attr;
	}

	std::string Object::getNameFromValue( Value* in_val ){
		return (*in_val)[ "req" ][ "attr" ][ "type" ][ "name" ].asString();
	}

	void Object::setParent( Object* parent_ptr ){
		parent = parent_ptr;		
	}

	void Object::setParentValue( Kal::Device* d, Value* in_val, GridsID parent_id ){		
		
		(*in_val)[ "parent" ] = parent_id;
	}
	
	Object* Object::getParentFromAttr( Kal::Device* d, Value* item_val ){
		Kal::Utility::puts( "Object::getParentFromAttr parent: ", getParentIDFromAttr(item_val) );
		
		GridsID temp_id = getParentIDFromAttr(item_val);
		if( temp_id.empty() )
			return NULL;
		
		void* temp_ptr = d->getInterface()->getObjectController()->getPointerFromID( temp_id );
		
		Kal::Utility::puts( (int)temp_ptr, getParentIDFromAttr(item_val) );
		
		return (Object*)temp_ptr;
	}
	
	GridsID Object::getParentIDFromAttr( Value* in_val ){
		return (*in_val)[ "parent" ].asString();
	} 


	Object* Object::getParentFromValue( Kal::Device* d, Value* item_val ){				
		return d->getInterface()->getObjectController()->getPointerFromID( getParentIDFromValue(item_val) );				
	}
	
	GridsID Object::getParentIDFromValue( Value* in_val ){
		return (*in_val)[ "req" ][ "attr" ][ "parent" ].asString();
	} 
	
	void Object::prepDraw(){

	}

	void Object::finDraw(){

	}	

	void Object::drawAll( Kal::Device* d ){
		lock();
		std::vector< Object* > temp_children = getChildren();

		for( int i = 0; i < temp_children.size(); i++ ){
			temp_children[i]->drawAll( d );
		}
		
		draw( d ); // draw yourself
	
		unlock();
	}
	
	std::vector< Object* > Object::getChildren(){			
		return children;
	}


	Vec3D Object::getPosition( ){
		Vec3D parents_position = Vec3D();
	
		if( parent )
			parents_position = parent->getPosition( );
		
		return getAttrPosition( ) + parents_position;
	}

	Vec3D Object::getAttrPosition( ){		
		Vec3D temp_position;

		lock();
		temp_position = Vec3D( attr[ "pos" ][ 0u ].asDouble(),
						   attr[ "pos" ][ 1u ].asDouble(),
						   attr[ "pos" ][ 2u ].asDouble() );
		unlock();
	
		return temp_position;
	}

	Vec3D Object::getScale(  ){
		Vec3D parents_scale = Vec3D(1.0f, 1.0f, 1.0f );
		
		if( parent )
			parents_scale = parent->getScale( );
				
		return getAttrScale( ) * parents_scale;
	}

	Vec3D Object::getAttrScale( ){				
		Vec3D temp_scale;

		lock();
		return Vec3D( attr[ "scl" ][ 0u ].asDouble(),
				    attr[ "scl" ][ 1u ].asDouble(),
				    attr[ "scl" ][ 2u ].asDouble() );
		unlock();
	
		return temp_scale;
	}


	Vec3D Object::getRotation( ){
		Vec3D parents_rot = Vec3D(0.0f, 0.0f, 0.0f );
		
		if( parent )
			parents_rot = parent->getRotation( );
			
		return getAttrRotation( ) * parents_rot;
	}

	Vec3D Object::getAttrRotation( ){		
		Vec3D temp_rotation;

		lock();
		temp_rotation = Vec3D( attr[ "rot" ][ 0u ].asDouble(),
						   attr[ "rot" ][ 1u ].asDouble(),
						   attr[ "rot" ][ 2u ].asDouble() );
		unlock();
	
		return temp_rotation;
	}

	
	void Object::setPosition( Kal::Device* d, Vec3D new_position ){
		d->getInterface()->getObjectController()->requestUpdatePosition( d, getID(), getRoomID(), new_position );
	}	

	void Object::setAttrPosition( Vec3D new_position ){
		lock();
		attr[ "pos" ][ 0u ] = new_position.X;
		attr[ "pos" ][ 1u ] = new_position.Y;
		attr[ "pos" ][ 2u ] = new_position.Z;
		unlock();
	}
	
	void Object::setAttrRotation( Vec3D new_rotation ){
		lock();
		attr[ "rot" ][ 0u ] = new_rotation.X;
		attr[ "rot" ][ 1u ] = new_rotation.Y;
		attr[ "rot" ][ 2u ] = new_rotation.Z;
		unlock();
	}

	void Object::setAttrScale( Vec3D new_scale ){
		lock();
		attr[ "scl" ][ 0u ] = new_scale.X;
		attr[ "scl" ][ 1u ] = new_scale.Y;
		attr[ "scl" ][ 2u ] = new_scale.Z;
		unlock();
	}


	void Object::storePositionFromAttr( Value* in_val ){		
		lock();
		attr[ "pos" ][ 0u ] = (*in_val)[ "pos" ][ 0u ].asDouble();
		attr[ "pos" ][ 1u ] = (*in_val)[ "pos" ][ 1u ].asDouble();
		attr[ "pos" ][ 2u ] = (*in_val)[ "pos" ][ 2u ].asDouble();

		attr[ "rot" ][ 0u ] = (*in_val)[ "rot" ][ 0u ].asDouble();
		attr[ "rot" ][ 1u ] = (*in_val)[ "rot" ][ 1u ].asDouble();
		attr[ "rot" ][ 2u ] = (*in_val)[ "rot" ][ 2u ].asDouble();

		attr[ "scl" ][ 0u ] = (*in_val)[ "scl" ][ 0u ].asDouble();
		attr[ "scl" ][ 1u ] = (*in_val)[ "scl" ][ 1u ].asDouble();
		attr[ "scl" ][ 2u ] = (*in_val)[ "scl" ][ 2u ].asDouble();
		unlock();		
	}
	
	
	GridsID Object::requestCreate( Kal::Device* d, GridsID parent, Value* in_val ){
		setParentValue( d, in_val, parent );
		
		//Utility::puts( "Requesting Create" );		
		return d->getInterface()->getObjectController()->requestCreateObject(d, in_val );
	}
	
	
	void Object::addChild( Object* item_ptr ) {
		lock();
		children.push_back( item_ptr );
		unlock();
	}

	void Object::deleteChild( Object* item_ptr ) {
		
	}

	void Object::deleteChildren( ){
		lock();
		std::vector< Object* > temp_children = getChildren();
	
		for( int i = 0; i < temp_children.size(); i++ ){
			delete temp_children[i];
		}

		unlock();
	}

	void Object::lock(){
		SDL_LockMutex( object_mutex );
	}

	void Object::unlock(){
		SDL_UnlockMutex( object_mutex );
	}
	
	void Object::initMutex(){
		object_mutex = SDL_CreateMutex();
	}

	void Object::deleteMutex() {
		SDL_DestroyMutex( object_mutex );
	}



} // end namespace Grids

