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


namespace Grids
{


	Object::Object( )
	{

	}

	void Object::requestUpdatePosition( Kaleidoscope::Device * d, Vec3D in_pos, Vec3D in_rot, Vec3D in_scl )
	// Called when an event is received
	{
		Value * temp_value = new Value();
		// generate actual value
		
		(*temp_value)[ "position" ][ 0u ] = in_pos.X;
		(*temp_value)[ "position" ][ 1u ] = in_pos.Y;
		(*temp_value)[ "position" ][ 2u ] = in_pos.Z;
		
		(*temp_value)[ "rotation" ][ 0u ] = in_rot.X;
		(*temp_value)[ "rotation" ][ 1u ] = in_rot.Y;
		(*temp_value)[ "rotation" ][ 2u ] = in_rot.Z;
		
		(*temp_value)[ "scale" ][ 0u ] = in_scl.X;
		(*temp_value)[ "scale" ][ 1u ] = in_scl.Y;
		(*temp_value)[ "scale" ][ 2u ] = in_scl.Z;
		
		d->getInterface()->getObjectController()->requestUpdateValue(d, this,  temp_value );
		
		delete temp_value;
	}

	void Object::updatePosition( Kaleidoscope::Device * d, Value * in_value )
	{
		// generate actual value
		
		//d->getInterface()->getObjectController()->updateValue( d, this, in_value );
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
		std::cerr << "Object method should exist in parent object" << std::endl;     
		
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

