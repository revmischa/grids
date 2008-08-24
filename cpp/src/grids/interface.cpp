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

	void Interface::sendEvent( std::string type, complex_type request)
	// Sends an event upstream
	{
		//protocol->sendRequest( type, request );
		std::cout << "Interface, attempting to send request" << std::endl;
		
		protocol->sendRequest( type );
	}
		

	void Interface::receiveEvent( Protocol * proto, Event * evt, void * userData )
	{
		std::cout << "Receive Event" << std::endl;
		( (Interface*)userData)->parseEventType( evt );
	}

	void Interface::connectionCallback(  Protocol * proto, Event *evt, void * userData )
	{
		std::cout << "callback" << std::endl;

		gridsmap_t m;
		m["message"] = "LOL HI";

		std::string e = "Debug.Warn";
		proto->sendRequest(e, &m);

		//((Interface*)userData)->creatRoom();
		
		if( evt )
		{
			((Interface*)userData)->parseEventType( evt );
		}
	}

	void Interface::parseEventType(  Event * evt )
	{
		std::cout << "parse event" << std::endl;

		std::string event_type = evt->getEventType();
		
		std::cout << evt->getEventType() << std::endl;
		std::cout << evt->getComplexType()[ "id" ].asString() << std::endl;

		if( event_type == "Room.Create" )
		{
			if( evt->getComplexType()[ "_method" ] == "Room.Create" )
			{
				// NOTA BENE: Even though Kaleidoscope doesn't send out a UUID method
				// It expects one back in the ID
				std::cout << "Created Room" << std::endl;
				
				d->getBuilder()->placeRoom( d,  evt->getComplexType()[ "id" ].asString() );
				d->getBuilder()->buildRoom( d,  evt->getComplexType()[ "id" ].asString() );
			}
		}
		else if( event_type == "Object.Place" )
		{
			if( evt->getComplexType()[ "_method" ] == "Object.Place" )
			{
				d->getBuilder()->placeObject( d, evt->getComplexType()[ "id" ].asString(),
						evt->getComplexType()[ "Room_ID" ].asString(),
						Vec3D(	evt->getComplexType()[ "Position" ][ "x" ].asDouble(),
								evt->getComplexType()[ "Position" ][ "y" ].asDouble(),
								evt->getComplexType()[ "Position" ][ "z" ].asDouble() ), 
						Vec3D(	evt->getComplexType()[ "Scale" ][ "x" ].asDouble(),
								evt->getComplexType()[ "Scale" ][ "y" ].asDouble(),
								evt->getComplexType()[ "Scale" ][ "z" ].asDouble() ), 
						Vec3D(	evt->getComplexType()[ "Rotation" ][ "x" ].asDouble(),
								evt->getComplexType()[ "Rotation" ][ "y" ].asDouble(),
								evt->getComplexType()[ "Rotation" ][ "z" ].asDouble() )		);
			}
		}
		
		
	}

	void Interface::createRoom( )
	{
		std::cout << "Attempting to add room" << std::endl;
		
		complex_type temp_type = complex_type();
		
		temp_type[ "_method" ] = "Room.Create";
		
		//protocol->sendRequest( "Kaleidoscope.Action", temp_type );
	}
	
	
	
	void Interface::setDevice( Kaleidoscope::Device * in_device )
	{
		d = in_device;
	}

	ObjectController * Interface::getObjectController() { return object_controller; }
	PersonController * Interface::getPersonController() { return person_controller; }
	MessengerController * Interface::getMessengerController() { return messenger_controller; }



} // end namespace Grids
