/*
 *  messengerController.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#pragma once

#include "interface.h"
#include "event.h"
#include "messenger.h"
#include <vector>
#include <map>

namespace Grids
{
	class Messenger;
	
	class MessengerController
	{
		public:
			MessengerController( );
			
			void setInterface( Interface * );
			
			void addMessenger( Messenger * );
			
			void sendEvent( Event * );
			
			void giveEvent( Event * );
			
		private:
			Interface * controller_interface;
			
			std::map< std::string, int > uuid_map;
			
			std::vector< Messenger * >  messengers;
	};
	
} // end namespace Grids
