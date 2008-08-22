/*
 *  person.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
 
 #pragma once
 
 #include <grids/event.h>
 #include <grids/personController.h>
 
 namespace Grids
 {
	class PersonController;
	
	class Person
	{
		public:
		
			Person( );
			
			void giveEvent( Event * );
			
			void sendEvent( std::string , complex_type  );
			
			void setController( PersonController * );
			
		private:
			
			PersonController * controller;
		
	};
 
 } // end namespace Grids

