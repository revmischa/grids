/*
 *  GridsInterface.cpp
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "interface.h"


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
		
		protocol->setEventCallback( receiveEvent );
	}
	
	void Interface::sendEvent( Event * evt )
	// Sends an event upstream
	{
		protocol->sendRequest( evt->getEventType(), evt->getMapPtr() );
	}
	
	void Interface::receiveEvent( Protocol * proto, Event * evt )
	{
		parseEventType( evt );
	}
	
	void Interface::parseEventType(  Event * evt )
	// This will be modified, person objects may need object information...
	// mayme all items need all information??
	{
		std::string event_type = evt->getEventType();
		
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
	
	ObjectController * Interface::getObjectController() { return object_controller; }
	PersonController * Interface::getPersenController() { return person_controller; }
	MessengerController * Interface::getMessengerController() { return messenger_controller; }
					


} // end namespace Grids