/*
 *  GridsInterface.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#pragma once

#include "GridsObjectController.h"
#include "GridsPersonController.h"
#include "GridsMessengerController.h"
#include "GridsEvent.h"
#include "GridsProtocol.h"
#include "GridsDefine.h"

namespace Grids
{
	
	class GInterface
	{
		public:
			
			GInterface( const char *, GObjectController *, GPersonController *, GMessengeController * );
			
			void sendEvent( GEvent );
			
		private:
			GObjectController * object_controller;
			GPersonController * person_controller;
			GMessengerController * messenger_controller;
			
			Protocol * protocol;
			
			char * node_address;
			
			void receiveEvent( Protocol *, GEvent * );
			void parseEventType( GEvent *);
			
	};

} // end namespace Grids