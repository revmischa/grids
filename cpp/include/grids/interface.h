/*
 *  GridsInterface.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#pragma once

#include <grids/define.h>
#include <grids/objectController.h>
#include <grids/personController.h>
#include <grids/messengerController.h>
#include <grids/event.h>
#include <grids/protocol.h>


namespace Grids
{

	//typedef void (*gevent_callback_t)(Protocol *, Event *);
	class ObjectController;
	class MessengerController;
	class PersonController;

	class Interface
	{
		public:

			Interface( char * );

			Interface( char *, ObjectController *, PersonController *, MessengerController * );

			~Interface( );

			void sendEvent( std::string , std::map< std::string, std::string > ); // Sends an event upstream with the grids protocol
			static void receiveEvent( Protocol *, Event *, void *  ); // Grids protocol object hooks into this
			static void connectionCallback(   Protocol *, Event *, void *  );


			ObjectController * getObjectController();
			PersonController * getPersonController();
			MessengerController * getMessengerController();

			void addRoom( );


		private:

			ObjectController * object_controller;
			PersonController * person_controller;
			MessengerController * messenger_controller;

			Protocol * protocol;

			char * node_address;

			void parseEventType( Event *);

	};

} // end namespace Grids

