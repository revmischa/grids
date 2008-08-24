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
#include <kaleidoscope/device.h>

#include <string>
#include <map>

namespace Kaleidoscope
{
	class Device;
}

namespace Grids
{

	//typedef void (*gevent_callback_t)(Protocol *, Event *);
	class ObjectController;
	class MessengerController;
	class PersonController;

	class Interface
	{
		public:

			Interface( Kaleidoscope::Device *, char * );

			Interface( Kaleidoscope::Device *, char *, ObjectController *, PersonController *, MessengerController * );

			~Interface( );

			void sendEvent( std::string , complex_type ); // Sends an event upstream with the grids protocol
			static void receiveEvent( Protocol *, Event *, void *  ); // Grids protocol object hooks into this
			static void connectionCallback(   Protocol *, Event *, void *  );


			ObjectController * getObjectController();
			PersonController * getPersonController();
			MessengerController * getMessengerController();
			
			void setDevice( Kaleidoscope::Device * );

			void createRoom( );

		private:
			
			Kaleidoscope::Device * d;

			ObjectController * object_controller;
			PersonController * person_controller;
			MessengerController * messenger_controller;

			Protocol * protocol;

			char * node_address;

			void parseEventType( Event *);

	};

} // end namespace Grids

