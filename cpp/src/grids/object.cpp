/*
 *  object.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <grids/object.h>


namespace Grids
{


	Object::Object( )
	{

	}

	void Object::requestUpdatePosition( Kaleidoscope::Device * d, Vec3D in_pos, Vec3D in_rot, Vec3D in_scl )
	// Called when an event is received
	{
		Value temp_value;
		// generate actual value
		
		d->getInterface()->getObjectController()->requestUpdateValue(d, this,  temp_value );

	}

	void Object::updatePosition( Kaleidoscope::Device * d, Vec3D in_pos, Vec3D in_rot, Vec3D in_scl )
	{
		Value temp_value;
		// generate actual value
		
		d->getInterface()->getObjectController()->updateValue( d, this, temp_value );
	}
	
	void Object::create( Kaleidoscope::Device * d, Value in_value )
	{
		
	}
	
	float Object::detectSelection( Kaleidoscope::Device * d, Vec3D ray_position, Vec3D ray_target )
	{
		return -1.0f;
	}
	
	void Object::selectObject( Kaleidoscope::Device * d )
	{
		
	}

	void Object::setController( ObjectController * cont )
	{
		controller = cont ;
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
	
	float Object::distanceToSphereAlongRay( Vec3D ray_pos, Vec3D ray_tar, Vec3D sphere_pos, float sphere_radius )
	{
		Vec3D q = sphere_pos - ray_pos;
		float c = q.getLength();
		float v = q.dotProduct(ray_tar);
		float d = sphere_radius * sphere_radius - (c * c - v * v);
		
		// If there was no intersection, return -1
		if (d < 0.0f)
			return -1;
		
		// Return the distance to the [first] intersecting point
		return v - (float) sqrt(d);             
		
		
	}



} // end namespace Grids

