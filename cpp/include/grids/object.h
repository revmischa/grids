/*
 *  GridsObject.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
 #pragma once
 
 #include <grids/event.h>
 #include <grids/objectController.h>
 
 namespace Grids
 {
	class ObjectController;
	
	class Object
	{
		public:
			
			Object( );
			
			void giveEvent( Event * );	// ObjectController uses this
										// to pass messages
			
			void sendEvent( std::string , std::map< std::string, std::string >  );									// upstream
										
			void setController( ObjectController * );
			
		private:
			
			ObjectController * controller;
	
	};
	
 } // end namespace Grids

