/*
 *  room.cpp
 *  kaleidoscope_2
 *
 *  Created by Patrick Tierney on 8/11/08.
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

#include <kaleidoscope/room.h>

namespace Kaleidoscope
{
	Room::Room( Device* d, Grids::Value* in_val ) : Object( d, in_val ) {

		if( d->getMyRoom() == "NULL" ){
			d->setMyRoom( getIDFromValue(in_val) );
		}

		d->getBuilder()->placeRoom( d,  this, getIDFromValue( in_val ) );
		d->getBuilder()->buildRoom( d,  this, getIDFromValue( in_val ) );
	}
	

	void Room::draw( Device* d ){


	}

	void Room::requestCreateRoom( Device* d, float room_size ){



	}
	
	void Room::create( Device* d, Grids::Value* in_val ){

	}
		
	Grids::Object* Room::getParentFromValue( Device* d, Grids::Value* in_val ){
		return NULL;
	}
	
	GridsID Room::getIDFromValue( Grids::Value* in_val ){
		Utility::puts( "Room getIDFromValue" );
		
		return (*in_val)[ "id" ].asString();
	}



		
} // end namespace Kaleidoscope
