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
	
	void ObjectController::setInterface( Interface * in_interface )
	{
		controller_interface = in_interface;
	}

	void ObjectController::sendCreateObject(  )
	{
		Value temp_value = Value();
		
		temp_value[ "_method" ] = "Room.Create" ;
		
		controller_interface->sendEvent( "Room.Create", temp_value );
	}
	
	void ObjectController::sendCreateObject( GridsID room_id )
	{
		std::string method = "Room.Object.Create";
		
		Value temp_value = Value();
		
		temp_value[ "_method" ] = method;
		//temp_value[ 
		
		controller_interface->sendEvent( method, temp_value );
	}
	
	void ObjectController::sendUpdatePosition( GridsID object_id, Vec3D new_position, Vec3D new_rotation, Vec3D new_scale )
	{
		
	}
	


	void ObjectController::sendEvent( std::string in_type, Value args )
	{
		controller_interface->sendEvent( in_type, args );
	}

	void ObjectController::giveEvent( Event * evt )
	{

		
	}

} // end namespace Grids

