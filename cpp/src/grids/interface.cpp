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
	Interface::Interface( char * address )
	{
		Interface( address, new ObjectController(), new PersonController(), new MessengerController() );
	}

	Interface::Interface(char * address, ObjectController * o_c_in, PersonController * p_c_in, MessengerController * m_c_in )
	//	: node_address( address), object_controller( o_c_in ), person_controller( p_c_in ), messenger_controller( m_c_in )
	{
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
		
		double start_time = clock();
		
		while( clock() - start_time < 1000 )
		{
		
		}

		protocol->runEventLoopThreaded();

	}

	Interface::~Interface()
	{
		protocol->stopEventLoopThread();
		protocol->closeConnection();
	}

	void Interface::sendEvent( std::string type, std::map< std::string, std::string > request)
	// Sends an event upstream
	{
		protocol->sendRequest( type );
	}

	void Interface::receiveEvent( Protocol * proto, Event * evt, void * userData )
	{
		( (Interface*)userData)->parseEventType( evt );
	}

	void Interface::connectionCallback(  Protocol * proto, Event *evt, void * userData )
	{
		std::cout << "callback";

		gridsmap_t m;
		m["message"] = "LOL HI";

		std::string e = "Debug.Warn";
		proto->sendRequest(e, &m);

		((Interface*)userData)->addRoom();
	}

	void Interface::parseEventType(  Event * evt )
	// This will be modified, person objects may need object information...
	// maybe all items need all information??
	{
		std::cout << "callback";

		std::string event_type = evt->getEventType();

		std::cout << event_type << std::endl;

		if( evt->getComplexType()[ "_method" ] == "Room.Create" )
		{
			std::cout << "Created Room" << std::endl;
		}

		if( event_type == "PERSON" )
		{
			person_controller->giveEvent( evt );
		}
		else if( event_type == "OBJECT" )
		{
			object_controller->giveEvent( evt );
		}
		else if( event_type == "MESSENGER" )
		{
			messenger_controller->giveEvent( evt );
		}
	}

	void Interface::addRoom( )
	{
		protocol->sendRequest( "Room.Create", NULL );
	}
	
	std::string Interface::addRoomDebug( Kaleidoscope::Device * d)
	{
		std::string new_id = "Room123456";
		
		d->rooms.push_back( new_id );
		
		return new_id;
	}

	ObjectController * Interface::getObjectController() { return object_controller; }
	PersonController * Interface::getPersonController() { return person_controller; }
	MessengerController * Interface::getMessengerController() { return messenger_controller; }



} // end namespace Grids
