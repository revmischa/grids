/*
 *  GridsInterface.h
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

		Interface( Kaleidoscope::Device *, std::string );

		Interface( Kaleidoscope::Device *, std::string, ObjectController *, PersonController *, MessengerController * );

		~Interface( );

		void sendEvent( std::string , Value * ); // Sends an event upstream with the grids protocol
		static void receiveEvent( Protocol *, Event *, void *  ); // Grids protocol object hooks into this
		static void connectionCallback(   Protocol *, Event *, void *  );

		ObjectController * getObjectController();
		PersonController * getPersonController();
		MessengerController * getMessengerController();

		void setDevice( Kaleidoscope::Device * );

		void createRoom( );

		std::string addRoomDebug( Kaleidoscope::Device * );
	
		void sendEventDebug( std::string, Value * );
		void receiveEventDebug( Protocol *, Event *, void * );
	
		void createRoomDebug();
		
		void lock( Kaleidoscope::Device * );
		void unlock( Kaleidoscope::Device * ); 
		
		void addObject( Kaleidoscope::Device *, std::string);
		void addObject( Kaleidoscope::Device *, Value *  );

	private:

		Kaleidoscope::Device * d;

		ObjectController * object_controller;
		PersonController * person_controller;
		MessengerController * messenger_controller;

		Protocol * protocol;

		std::string node_address;

		void parseEventType( Event *);

	};

} // end namespace Grids

