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
	class MessengerController
	{
		public:
			MessengerController( );
			
			addMessenger( Messenger * );
			
			sendEvent( Event * );
			
			giveEvent( Event * );
			
		private:
			Interface * interface;
			
			map< std::string, int > uuid_map;
			
			vector< Messenger * >  messengers;
	};
	
} // end namespace Grids