/*
 *  GridsInterface.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#pragma once

#include "objectController.h"
#include "personController.h"
#include "messengerController.h"
#include "event.h"
#include "protocol.h"
#include "define.h"

namespace Grids
{
	
	//typedef void (*gevent_callback_t)(Protocol *, Event *);
	
	class Interface
	{
		public:
			
			Interface( const char * );
			
			Interface( const char *, ObjectController *, PersonController *, MessengerController * );
			
			~Interface( );
			
			void sendEvent( Event *); // Sends an event upstream with the grids protocol
			void receiveEvent( Protocol *, Event * ); // Grids protocol object hooks into this

			ObjectController * getObjectController();
			PersonController * getPersonController();
			MessengerController * getMessengerController();
			
		private:
		
			ObjectController * object_controller;
			PersonController * person_controller;
			MessengerController * messenger_controller;
			
			Protocol * protocol;
			
			const char * node_address;
			
			void parseEventType( Event *);
			
	};

} // end namespace Grids