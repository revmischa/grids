/*
 *  messenger.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
 
 #pragma once
 
 #include "event.h"
 #include "messengerController.h"
 
 
 namespace Grids
 {
	class MessengerController;
	
	class Messenger
	{
		public:
			
			Messenger( );
			
			void giveEvent( Event * );
			
			void sendEvent( Event * );
			
		private:
			
			MessengerController * controller;
	
	};

} // end namespace Grids

