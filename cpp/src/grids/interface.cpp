/*
 *  GridsInterface.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <grids/interface.h>
#include <json/value.h>

#include <time.h>
#include <iostream>
#include <map>



namespace Grids
{
	Interface::Interface( Kaleidoscope::Device * in_device, char * address )
	{
		Interface(  in_device, address, new ObjectController(), new PersonController(), new MessengerController() );
	}

	Interface::Interface( Kaleidoscope::Device * in_device, char * address, ObjectController * o_c_in, PersonController * p_c_in, MessengerController * m_c_in )
	{
		d = in_device;
		node_address = address;
		object_controller = o_c_in;
		person_controller = p_c_in;
		messenger_controller = m_c_in;

		object_controller->setInterface( this );
		person_controller->setInterface( this );
		messenger_controller->setInterface( this );

		protocol = new Protocol();

		protocol->setConnectedCallback( &connectionCallback, this );
		protocol->setEventCallback( &receiveEvent, this );

		if (! protocol->connectToNode( node_address ) )
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

		delete object_controller;
		delete person_controller;
		delete messenger_controller;

		protocol->stopEventLoopThread();
		protocol->closeConnection();

		delete protocol;
	}

	void Interface::sendEvent( std::string type, Value request)
	// Sends an event upstream
	{
		std::cout << "Interface, attempting to send request" << std::endl;

		protocol->sendRequest( type );
	}


	void Interface::receiveEvent( Protocol * proto, Event * evt, void * self )
	{
		std::cout << "Receive Event" << std::endl;
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
	{
		std::cout << "parse event" << std::endl;

		std::string event_type = evt->getEventType();

		std::cout << evt->getEventType() << std::endl;
		std::cout << evt->getArgs()[ "id" ].asString() << std::endl;

		if( event_type == "Room.Create" )
		{
			// NOTA BENE: Even though Kaleidoscope doesn't send out a UUID method
			// It expects one back in the ID
			std::cout << "Created Room" << std::endl;

			d->getBuilder()->placeRoom( d,  evt->getArgs()[ "id" ].asString() );
			d->getBuilder()->buildRoom( d,  evt->getArgs()[ "id" ].asString() );
		}
		else if( event_type == "Object.Place" )
		{
			d->getBuilder()->placeObject( d, evt->getArgs()[ "id" ].asString(),
					evt->getArgs()[ "roomId" ].asString(),
					Vec3D(	evt->getArgs()[ "position" ][ 0u ].asDouble(),
							evt->getArgs()[ "position" ][ 1u ].asDouble(),
							evt->getArgs()[ "position" ][ 2u ].asDouble() ),
					Vec3D(	evt->getArgs()[ "scale" ][ 0u ].asDouble(),
							evt->getArgs()[ "scale" ][ 1u ].asDouble(),
							evt->getArgs()[ "scale" ][ 2u ].asDouble() ),
					Vec3D(	evt->getArgs()[ "rotation" ][ 0u ].asDouble(),
							evt->getArgs()[ "rotation" ][ 1u ].asDouble(),
							evt->getArgs()[ "rotation" ][ 2u ].asDouble() )		);
		}
		else if( event_type == "Room.Object.Create" )
		{
		
//			[ "roomId" ] = 
//			[ "attr" ] = 
//			Room.Object.Create = 
//			Room.Object.Update = 

		}
		else if( event_type == "Room.Object.Update" )
		{
		
		}

	}

	void Interface::createRoom( )
	{
		std::cout << "Attempting to add room" << std::endl;

		Value temp_type = Value();

		temp_type[ "_method" ] = "Room.Create";

		sendEvent( "Room.Create", temp_type );
	}
	
	
	/////////////////////
	//// OTHER METHODS
	//////////////////
	
	// Create Object
	// Pass Room_Id, ( or none ) at the very least
	
	
	// Update Object (specify ID)
	// Pass a new location, geometry, texture, etc
	
	//

	void Interface::setDevice( Kaleidoscope::Device * in_device )
	{
		d = in_device;
	}

	ObjectController * Interface::getObjectController() { return object_controller; }
	PersonController * Interface::getPersonController() { return person_controller; }
	MessengerController * Interface::getMessengerController() { return messenger_controller; }



} // end namespace Grids
