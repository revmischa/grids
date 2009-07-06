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
#include <kaleidoscope/RenderObject.h>

#include <grids/object.h>

namespace Kaleidoscope
{
	Room::Room( Device* d, Grids::Value* in_val ) : GLSpaceObject( d, in_val ) {

		if( d->getMyRoom() == "NULL" ){
			d->setMyRoom( getIDFromValue(in_val) );
		}

		//setID( getIDFromValue(in_val) );
		
		hide(); // Don't draw it until we've built it

		Utility::puts( "Hiding renderer" );		
		d->getRenderer()->hide();
		
		setParent( d->getRenderer() );
		Utility::puts( "placing room" );		
		placeRoom( d,  this );  
		d->getRenderer()->show();
				
		lock();
		buildRoom( d,  this );
		unlock();

		d->getRenderer()->addChild( this );
		//setParent( NULL );
		
		show();
		
		Utility::puts( "Created room id = ", ((Grids::Object*)this)->getID() );

		Utility::puts( "Created room parent = ", (unsigned int)parent );		
	}	

	void Room::draw( Device* d ){
		glDraw( d );
	}

	void Room::requestCreateRoom( Device* d, float room_size ){

	}
	
	void Room::create( Device* d, Grids::Value* in_val ){

	}
		
	Grids::Object* Room::getParentFromValue( Device* d, Grids::Value* in_val ){
		return NULL;
	}
	
	Grids::GridsID Room::getIDFromValue( Grids::Value* in_val ){
		Utility::puts( "Room getIDFromValue" );
		
		return (*in_val)[ "id" ].asString();
	}

	void Room::placeRoom( Device * d, Room* in_room )
	{
		GridsID new_id = in_room->getID();
		
		Utility::puts( "Placing room = ", new_id );
		
		float room_width = d->getRoomWidth();
		
		d->lockWorldHash();
		bool rooms_dont_exist = !( d->world_hash[ "rooms" ] );
		d->unlockWorldHash();
			
		if( rooms_dont_exist ){						
			Utility::puts( "*****First Room******" );

			d->lockWorldHash();
			d->world_hash[ "rooms" ][ 0u ] = new_id;
			d->unlockWorldHash();

			in_room->lock();
			in_room->attr[ "pos" ][ 0u ] = 0.0f;
			in_room->attr[ "pos" ][ 1u ] = 0.0f;
			in_room->attr[ "pos" ][ 2u ] = 0.0f;
			in_room->attr[ "scl" ][ 0u ] = 1.0f;
			in_room->attr[ "scl" ][ 1u ] = 1.0f;
			in_room->attr[ "scl" ][ 2u ] = 1.0f;
			in_room->attr[ "rot" ][ 0u ] = 0.0f;
			in_room->attr[ "rot" ][ 1u ] = 0.0f;
			in_room->attr[ "rot" ][ 2u ] = 0.0f;
			in_room->unlock();
		}
		else if( false ){
			int num_rooms = d->world_hash[ "rooms" ].size();
			
			d->lockWorldHash();
			d->world_hash[ "rooms" ][ num_rooms ] = new_id;
			d->unlockWorldHash();

			in_room->lock();
			in_room->attr[ "pos" ][ 0u ] = num_rooms * 200.0f;
			in_room->attr[ "pos" ][ 1u ] = 0.0f;
			in_room->attr[ "pos" ][ 2u ] = 0.0f;
			in_room->attr[ "scl" ][ 0u ] = 1.0f;
			in_room->attr[ "scl" ][ 1u ] = 1.0f;
			in_room->attr[ "scl" ][ 2u ] = 1.0f;
			in_room->attr[ "rot" ][ 0u ] = 0.0f;
			in_room->attr[ "rot" ][ 1u ] = 0.0f;
			in_room->attr[ "rot" ][ 2u ] = 0.0f;
			in_room->unlock();
		}
		else{
			Utility::puts( "*****Other  Room******" );

			int world_size = 5; // 20 x 20 x 20 = 8000 total rooms
			
			// The default position is the position to place new rooms if all of the rooms are filled up
			// It is the farthest possible position
			Vec3D default_position = Vec3D( -world_size * room_width * 2,
									  -world_size * room_width * 2,
									  -world_size * room_width * 2 );
			
			Vec3D closest_position = default_position; 

			Vec3D temp_position;
			
			for( int i = -world_size; i < world_size; i++) {
				for( int g = -world_size; g < world_size; g++) {
					for( int h = -world_size; h < world_size; h++) {
						temp_position = Vec3D( i * 2 * room_width, g * 2 * room_width, h * 2 * room_width );
						
						d->lockWorldHash();
						int j_max = d->world_hash[ "rooms" ].size();
						d->unlockWorldHash();

						for( int j = 0; j < j_max; j++ ) {
							d->lockWorldHash();
							GridsID temp_id = d->world_hash[ "rooms" ][ j ].asString();
							d->unlockWorldHash();
							
							Grids::Object* temp_object = d->getInterface()->getObjectController()->getPointerFromID( temp_id );
							
							//std::cout << j << "  "  << temp_id << " = " << (unsigned int)temp_object << std::endl;
							Utility::puts( "Getting position" );
							//Vec3D temp_room_position = temp_object->getPosition();
							Vec3D temp_room_position = temp_object->getAttrPosition();
							Utility::puts( "Got position" );

							
							if( temp_position == temp_room_position ){
								temp_position = default_position;
							}
							
						} // end for Rooms
						
						
						if( temp_position.getLength() < closest_position.getLength() ){
							closest_position = temp_position;
						}						
					} // end for h
				} // end for g
			} // end for i

			d->lockWorldHash();

			unsigned int num_rooms = d->world_hash[ "rooms" ].size();
			d->world_hash[ "rooms" ][ num_rooms ] = new_id;

			d->unlockWorldHash();

			
			in_room->lock();
			in_room->attr[ "pos" ][ 0u ] = closest_position.X;
			in_room->attr[ "pos" ][ 1u ] = closest_position.Y;
			in_room->attr[ "pos" ][ 2u ] = closest_position.Z;
			in_room->attr[ "scl" ][ 0u ] = 1.0f;
			in_room->attr[ "scl" ][ 1u ] = 1.0f;
			in_room->attr[ "scl" ][ 2u ] = 1.0f;
			in_room->attr[ "rot" ][ 0u ] = 0.0f;
			in_room->attr[ "rot" ][ 1u ] = 0.0f;
			in_room->attr[ "rot" ][ 2u ] = 0.0f;
			in_room->unlock();
			
			Utility::puts( "Placed room @ ", closest_position );
		} // end else


		
	} // end placeRoom

	void Room::buildRoom( Device * d, Room* in_room )
	{
		GridsID new_id = in_room->getID();

		float room_width = d->getRoomWidth();
		int num_lines = room_width;
		
		in_room->lock();

		// Lines Color
		in_room->attr[ "color" ][ 0u ][ 0u ] = 1.0f;
		in_room->attr[ "color" ][ 0u ][ 1u ] = 1.0f;
		in_room->attr[ "color" ][ 0u ][ 2u ] = 1.0f;
		in_room->attr[ "color" ][ 0u ][ 3u ] = 0.1f;

		in_room->attr[ "lines" ][ 0u ][ "indices" ] = Grids::Value();
		in_room->attr[ "lines" ][ 0u ][ "color" ] = 0u;

		in_room->attr[ "vertices" ] = Grids::Value();


		for(int i = 0; i <= num_lines * 2; i += 4 ) // i * 2 from -num_lines to num_lines --  + 4 Vertices
		{
			in_room->attr[ "vertices" ][ i ] = Grids::Value();

			in_room->attr[ "vertices" ][ i ][ 0u ] = (float)i - room_width;
			in_room->attr[ "vertices" ][ i ][ 1u ] = 0.0f;
			in_room->attr[ "vertices" ][ i ][ 2u ] = (float)-num_lines;

			in_room->attr[ "vertices" ][ i+1 ] = Grids::Value();

			in_room->attr[ "vertices" ][ i+1 ][ 0u ] = (float)i - room_width;
			in_room->attr[ "vertices" ][ i+1 ][ 1u ] = 0.0f;
			in_room->attr[ "vertices" ][ i+1 ][ 2u ] = (float)num_lines ;

			in_room->attr[ "vertices" ][ i+2 ] = Grids::Value();

			in_room->attr[ "vertices" ][ i+2 ][ 0u ] = (float)num_lines;
			in_room->attr[ "vertices" ][ i+2 ][ 1u ] = 0.0f;
			in_room->attr[ "vertices" ][ i+2 ][ 2u ] = (float)i - room_width;

			in_room->attr[ "vertices" ][ i+3 ] = Grids::Value();

			in_room->attr[ "vertices" ][ i+3 ][ 0u ] = (float)-num_lines;
			in_room->attr[ "vertices" ][ i+3 ][ 1u ] = 0.0f;
			in_room->attr[ "vertices" ][ i+3 ][ 2u ] = (float)i - room_width;
		}

		// Indices are ordered pairs ( or triplets, or quintuplets ) of numbers that indicate whicu vertices belong to which line, triangle, or quadrilateral

		int j = 0;

		for( int i = 0; i < num_lines + 1 ; i += 2 )
		{
			in_room->attr[ "lines" ][ 0u ][ "indices" ][ i ] = Grids::Value();

			in_room->attr[ "lines" ][ 0u ][ "indices" ][ i ][ 0u ] = j;
			in_room->attr[ "lines" ][ 0u ][ "indices" ][ i ][ 1u ] = j + 1;

			in_room->attr[ "lines" ][ 0u ][ "indices" ][ i + 1 ] = Grids::Value();

			in_room->attr[ "lines" ][ 0u ][ "indices" ][ i + 1 ][ 0u ] = j + 2;
			in_room->attr[ "lines" ][ 0u ][ "indices" ][ i + 1 ][ 1u ] = j + 3;

			j += 4;
		}

		// Wall Color

		in_room->attr[ "color" ][ 1u ] = Grids::Value();
		in_room->attr[ "color" ][ 1u ][ 0u ] = 1.0f;
		in_room->attr[ "color" ][ 1u ][ 1u ] = 1.0f;
		in_room->attr[ "color" ][ 1u ][ 2u ] = 1.0f;
		in_room->attr[ "color" ][ 1u ][ 3u ] = 0.01f;

		// Outline Color -- faint white
		in_room->attr[ "color" ][ 2u ] = Grids::Value();
		in_room->attr[ "color" ][ 2u ][ 0u ] = 1.0f;
		in_room->attr[ "color" ][ 2u ][ 1u ] = 1.0f;
		in_room->attr[ "color" ][ 2u ][ 2u ] = 1.0f;
		in_room->attr[ "color" ][ 2u ][ 3u ] = 0.15f;

		in_room->attr[ "quads" ][ 0u ] = Grids::Value();
		in_room->attr[ "quads" ][ 0u ][ "indices" ] = Grids::Value();
		in_room->attr[ "quads" ][ 0u ][ "color" ] = 1; // Use color # 1 to shade this quad

		int k = in_room->attr[ "vertices" ].size();

		for( int i = k; i < k + 8; i++ ){
			in_room->attr[ "vertices" ][ i ] = Grids::Value();
		}

		in_room->attr[ "vertices" ][ k ][ 0u ] = -room_width;
		in_room->attr[ "vertices" ][ k ][ 1u ] = -room_width;
		in_room->attr[ "vertices" ][ k ][ 2u ] = -room_width;

		in_room->attr[ "vertices" ][ k + 1 ][ 0u ] = room_width;
		in_room->attr[ "vertices" ][ k + 1 ][ 1u ] = -room_width;
		in_room->attr[ "vertices" ][ k + 1 ][ 2u ] = -room_width;

		in_room->attr[ "vertices" ][ k + 2 ][ 0u ] = room_width;
		in_room->attr[ "vertices" ][ k + 2 ][ 1u ] = room_width;
		in_room->attr[ "vertices" ][ k + 2 ][ 2u ] = -room_width;

		in_room->attr[ "vertices" ][ k + 3 ][ 0u ] = -room_width;
		in_room->attr[ "vertices" ][ k + 3 ][ 1u ] = room_width;
		in_room->attr[ "vertices" ][ k + 3 ][ 2u ] = -room_width;

		in_room->attr[ "vertices" ][ k + 4 ][ 0u ] = -room_width;
		in_room->attr[ "vertices" ][ k + 4 ][ 1u ] = -room_width;
		in_room->attr[ "vertices" ][ k + 4 ][ 2u ] = room_width;

		in_room->attr[ "vertices" ][ k + 5 ][ 0u ] = room_width;
		in_room->attr[ "vertices" ][ k + 5 ][ 1u ] = -room_width;
		in_room->attr[ "vertices" ][ k + 5 ][ 2u ] = room_width;

		in_room->attr[ "vertices" ][ k + 6 ][ 0u ] = room_width;
		in_room->attr[ "vertices" ][ k + 6 ][ 1u ] = room_width;
		in_room->attr[ "vertices" ][ k + 6 ][ 2u ] = room_width;

		in_room->attr[ "vertices" ][ k + 7 ][ 0u ] = -room_width;
		in_room->attr[ "vertices" ][ k + 7 ][ 1u ] = room_width;
		in_room->attr[ "vertices" ][ k + 7 ][ 2u ] = room_width;

		for( int i = 0; i < 6; i++ ){
			in_room->attr[ "quads" ][ 0u ][ "indices" ][ i ] = Grids::Value();
		}

		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 0u ][ 0u ] = k + 0u; // Box top
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 0u ][ 1u ] = k + 1u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 0u ][ 2u ] = k + 2u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 0u ][ 3u ] = k + 3u;

		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 1u ][ 0u ] = k + 4u; // Box bottom
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 1u ][ 1u ] = k + 5u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 1u ][ 2u ] = k + 6u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 1u ][ 3u ] = k + 7u;

		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 2u ][ 0u ] = k + 4u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 2u ][ 1u ] = k + 5u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 2u ][ 2u ] = k + 1u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 2u ][ 3u ] = k + 0u;

		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 3u ][ 0u ] = k + 7u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 3u ][ 1u ] = k + 6u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 3u ][ 2u ] = k + 2u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 3u ][ 3u ] = k + 3u;

		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 4u ][ 0u ] = k + 4u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 4u ][ 1u ] = k + 7u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 4u ][ 2u ] = k + 3u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 4u ][ 3u ] = k + 0u;

		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 5u ][ 0u ] = k + 5u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 5u ][ 1u ] = k + 6u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 5u ][ 2u ] = k + 2u;
		in_room->attr[ "quads" ][ 0u ][ "indices" ][ 5u ][ 3u ] = k + 1u;


		// Do the faint white outline lines

		in_room->attr[ "lines" ][ 1u ][ "color" ] = 2u;

		for( int i = 0; i < 12; i++ ){
			in_room->attr[ "lines" ][ 1u ][ "indices" ][ i ] = Grids::Value();
		}

		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 0u ][ 0u ] = k + 0u;
		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 0u ][ 1u ] = k + 1u;

		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 1u ][ 0u ] = k + 3u;
		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 1u ][ 1u ] = k + 2u;

		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 2u ][ 0u ] = k + 4u;
		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 2u ][ 1u ] = k + 5u;

		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 3u ][ 0u ] = k + 7u;
		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 3u ][ 1u ] = k + 6u;

		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 4u ][ 0u ] = k + 0u;
		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 4u ][ 1u ] = k + 3u;

		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 5u ][ 0u ] = k + 1u;
		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 5u ][ 1u ] = k + 2u;

		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 6u ][ 0u ] = k + 4u;
		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 6u ][ 1u ] = k + 7u;

		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 7u ][ 0u ] = k + 5u;
		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 7u ][ 1u ] = k + 6u;

		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 8u ][ 0u ] = k + 4u;
		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 8u ][ 1u ] = k + 0u;

		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 9u ][ 0u ] = k + 7u;
		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 9u ][ 1u ] = k + 3u;

		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 10u ][ 0u ] = k + 5u;
		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 10u ][ 1u ] = k + 1u;

		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 11u ][ 0u ] = k + 6u;
		in_room->attr[ "lines" ][ 1u ][ "indices" ][ 11u ][ 1u ] = k + 2u;
		
		in_room->unlock();
		
	} // end BuildRoom


		
} // end namespace Kaleidoscope
