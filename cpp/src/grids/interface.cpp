/*
 *  GridsInterface.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
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

#include <grids/interface.h>
#include <json/value.h>
#include <kaleidoscope/simpleCube.h>

#include <time.h>
#include <iostream>
#include <map>
#include <sstream>



namespace Grids
{
	Interface::Interface( Kaleidoscope::Device * in_device, std::string address )
	{
		Interface(  in_device, address, new ObjectController(), new PersonController(), new MessengerController() );
	}

	Interface::Interface( Kaleidoscope::Device * in_device, std::string address, ObjectController * o_c_in, PersonController * p_c_in, MessengerController * m_c_in )
	{
		d = in_device;
		node_address = address;
		object_controller = o_c_in;
		person_controller = p_c_in;
		messenger_controller = m_c_in;

		person_controller->setInterface( this );
		messenger_controller->setInterface( this );

		protocol = new Protocol();

		protocol->setConnectedCallback( &connectionCallback, this );
		protocol->setEventCallback( &receiveEvent, this );

		if (! protocol->connectToNode( node_address.c_str() ) )
		{
			std::cerr << "Could not connect to " << node_address << "\n";
			return;
		}

		std::cout << "Connected to " << node_address << std::endl;

		protocol->runEventLoopThreaded();

	}

	Interface::~Interface()
	{
		std::cout << "Destroying Interface" << std::endl;

		if( object_controller )
			delete object_controller;
		
		if( person_controller )
			delete person_controller;
		
		if( messenger_controller )
			delete messenger_controller;

		protocol->stopEventLoopThread();
		protocol->closeConnection();

		delete protocol;
	}

	void Interface::sendEvent( std::string type, Value * request)
	// Sends an event upstream
	{
		std::cout << "Interface, attempting to send request" << std::endl;
		std::cout << (*request)[ "room_id" ].asString() << std::endl;

		protocol->sendRequest( type, request );
	}
	
	void Interface::sendEventDebug( std::string type, Value * request )
	{		
		Event * temp_event = new Event();
		
		GridsID temp_id;
		std::stringstream temp_stream;
		
		temp_stream << SDL_GetTicks()*rand();
		
		temp_id = temp_stream.str();
		
		temp_event->setEvent( (*request)[ "_method" ].asString() );
		
		(*request)[ "id" ] = temp_id;
		
		temp_event->setArgs( *request );
		
		receiveEventDebug( NULL, temp_event , NULL );
		
		delete temp_event;
		
	}
	
	void Interface::receiveEventDebug( Protocol * proto, Event * evt, void * self )
	{
		parseEventType( evt );
	}

	void Interface::receiveEvent( Protocol * proto, Event * evt, void * self )
	{
		//std::cout << "Receive Event" << std::endl;
		( (Interface*)self)->parseEventType( evt );
	}

	void Interface::connectionCallback(  Protocol * proto, Event *evt, void * self )
	{
		std::cout << "callback" << std::endl;

		Value m;
		m["message"] = "LOL HI";

		std::string e = "Debug.Warn";
		proto->sendRequest(e, &m);
	}

	void Interface::parseEventType(  Event * evt )
	// NOTE: This call runs inside of its own thread, started by Grids protocol
	{
		std::string event_type = evt->getEventType();
		
		std::cout << "Parse Event Type: " << event_type << std::endl;
		std::cout << "Parse ID: " << evt->getArgs()[ "id" ].asString() << std::endl;
		std::cout << "Parse Method: " << evt->getArgs()[ "_method" ].asString() << std::endl;
		std::cout << "Parse size(): " << evt->getArgs().size() << std::endl;

		if( event_type == GRIDS_CREATE_ROOM )
		{
			d->getBuilder()->placeRoom( d,  evt->getArgs()[ "id" ].asString() );
			d->getBuilder()->buildRoom( d,  evt->getArgs()[ "id" ].asString() );
			
			d->getBuilder()->createRandomBoxes(d, evt->getArgs()[ "id" ].asString(), 20);
			
			//d->getVoxel()->update(d, 3, 0.45f);
		}
		else if( event_type == GRIDS_CREATE_OBJECT )
		{			
			object_controller->createObject( d, evt->getArgs() );

		}
		else if( event_type == GRIDS_UPDATE_OBJECT )
		{
			object_controller->updateValue( d, evt->getArgs()[ "id" ].asString(), evt->getArgs() );
		}
		
	}

	void Interface::createRoom( )
	{
		std::cout << "Attempting to add room" << std::endl;

		Value * temp_type = new Value();

		(*temp_type)[ "_method" ] = GRIDS_CREATE_ROOM;

		sendEvent(  GRIDS_CREATE_ROOM, temp_type );
		
		delete temp_type;
	}
	
	void Interface::createRoomDebug()
	{
		
		Event temp_event;
		// Simplate the actions of the server
		Value request = Value();
		
		GridsID temp_id;
		std::stringstream temp_stream;
		
		temp_stream << SDL_GetTicks()*rand();
		temp_id = temp_stream.str();
		
		temp_event.setEvent( GRIDS_CREATE_ROOM );
		
		request[ "id" ] = temp_id;
		
		temp_event.setArgs( request );
		
		parseEventType( &temp_event );
		
	}
	
	
	/////////////////////
	//// OTHER METHODS
	//////////////////
	
	// Create Object
	// SEND		==>	GridsID of room ( or none ) 
	// RECEIVE	==>	GridsID of new object, GridsID of room if applicable
	//
	//	
	// Update Object Position
	// SEND		==>	GridsID of object, Position Vector, Rotation Vector, Scale Vector
	// RECEIVE	==>	GridsID of object, Position Vector, Rotation Vector, Scale Vector
	//
	//
	// Create Person
	// SEND		==> GridsID of room
	// RECEIVE	==>	GridsID of person, GridsID of person's room
	//
	// Update Person Position
	// SEND		==> GridsID of person, Position Vector, Rotation Vector, Scale Vector
	// RECEIVE	==>	GridsID of object, Position Vector, Rotation Vector, Scale Vector
	//
	// maybe Update Object Position and Update Person Position can be combined?
	
	
	
	// Update person ( 

	void Interface::setDevice( Kaleidoscope::Device * in_device )
	{
		d = in_device;
	}

	ObjectController * Interface::getObjectController() { return object_controller; }
	PersonController * Interface::getPersonController() { return person_controller; }
	MessengerController * Interface::getMessengerController() { return messenger_controller; }
	
	
	void Interface::lock( Kaleidoscope::Device * d )
	{
		SDL_mutexP( d->interface_mutex );
	}
	
	void Interface::unlock( Kaleidoscope::Device * d )
	{
		SDL_mutexV( d->interface_mutex );
	}



} // end namespace Grids
