/*
 *  GridsObject.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
 #pragma once
 
 #include "event.h"
 #include "objectController.h"
 
 namespace Grids
 {
	class ObjectController;
	
	class Object
	{
		public:
			
			Object( );
			
			void giveEvent( Event * );	// ObjectController uses this
										// to pass messages
			
			void sendEvent( Event * );  // Used to send a message 
										// upstream
			
		private:
			
			ObjectController * controller;
	
	};
	
 } // end namespace Grids
