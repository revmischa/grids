/*
 *  person.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/8/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */
 
 
 #pragma once
 
 #include "event.h"
 #include "personController.h"
 
 namespace Grids
 {
	class PersonController;
	
	class Person
	{
		public:
		
			Person( );
			
			void giveEvent( Event * );
			
			void sendEvent( Event * );
			
		private:
			
			PersonController * controller;
		
	};
 
 } // end namespace Grids

