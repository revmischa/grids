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
#include "GridsProtocol.h"
#include "define.h"

namespace Grids
{
	
	class Interface
	{
		public:
			
			Interface( const char * );
			
			Interface( const char *, ObjectController *, PersonController *, MessengerController * );
			
			
			void sendEvent( Event *); // Sends an event upstream with the grids protocol
			
			ObjectController * getObjectController();
			PersonController * getPersenController();
			MessengerController * getMessengerController();
			
		private:
		
			ObjectController * object_controller;
			PersonController * person_controller;
			MessengerController * messenger_controller;
			
			Protocol * protocol;
			
			const char * node_address;
			
			void receiveEvent( Protocol *, Event * );
			void parseEventType( Event *);
			
	};

} // end namespace Grids