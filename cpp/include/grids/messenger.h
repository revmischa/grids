/*
 *  messenger.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
 
 #pragma once
 
 #include <grids/event.h>
 #include <grids/messengerController.h>
 
 
 namespace Grids
 {
	class MessengerController;
	
	class Messenger
	{
		public:
			
			Messenger( );
			
			void giveEvent( Event * );
			
			void sendEvent( std::string in_type, std::map< std::string, std::string > complex_value );
						
			void setController( MessengerController * );
			
		private:
			
			MessengerController * controller;
	
	};

} // end namespace Grids

