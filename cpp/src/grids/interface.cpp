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
#include <iostream>



namespace Grids
{
	Interface::Interface( const char * address )
	{
		Interface( address, new ObjectController(), new PersonController(), new MessengerController() ); 
	}
	
	Interface::Interface( const char * address, ObjectController * o_c_in, PersonController * p_c_in, MessengerController * m_c_in )
		: node_address( address), object_controller( o_c_in ), person_controller( p_c_in ), messenger_controller( m_c_in )
	{
		object_controller->setInterface( this );
		person_controller->setInterface( this );
		messenger_controller->setInterface( this );
		
		protocol = new Protocol();
		
		while( !protocol->connectToNode( node_address ) )
		{
			// This should probably be threaded in the future
		}
		
		std::cout << "Connected!" << std::endl;
		
		protocol->setEventCallback( &Grids::Interface::receiveEvent, this );
		protocol->runEventLoopThreaded();
		
		gridsmap_t m;
		m["message"] = "LOL HI";
		
		
		std::string evt = "Debug.Warn";
		protocol->sendRequest(evt, &m);		
		
		//wait for some time
		for( double i = 0; i < 1000000000; i++)
		{
		}
		
		addRoom();
	}
	
	Interface::~Interface()
	{
		protocol->stopEventLoopThread();
		protocol->closeConnection();
	}
	
	void Interface::sendEvent( Event * evt )
	// Sends an event upstream
	{
		protocol->sendRequest( evt->getEventType(), evt->getMapPtr() );
	}
	
	void Interface::receiveEvent( Protocol * proto, Event * evt, void * userData )
	{
		( (Interface*)userData)->parseEventType( evt );
	}
	
	void Interface::parseEventType(  Event * evt )
	// This will be modified, person objects may need object information...
	// maybe all items need all information??
	{
		std::string event_type = evt->getEventType();
		
		std::cout << event_type << std::endl;
		
		if( evt->getMap()[ "_method" ] == "Room.Create" )
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
	
	ObjectController * Interface::getObjectController() { return object_controller; }
	PersonController * Interface::getPersonController() { return person_controller; }
	MessengerController * Interface::getMessengerController() { return messenger_controller; }
					


} // end namespace Grids
