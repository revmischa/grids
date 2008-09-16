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
		d->getInterface()->getObjectController()->requestUpdatePosition(d, this, in_pos, in_rot, in_scl);

	}

	void Object::updatePosition( Kaleidoscope::Device * d, Vec3D in_pos, Vec3D in_rot, Vec3D in_scl )
	{
		d->getInterface()->getObjectController()->updatePosition( d, this, in_pos, in_rot, in_scl );
	}
	
	void Object::create( Kaleidoscope::Device * d, Value cube_value )
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



} // end namespace Grids

