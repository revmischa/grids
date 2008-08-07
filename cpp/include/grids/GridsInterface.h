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
#include "GridsMessageController.h"
#include "GridsEvent.h"
#include "GridsProtocol.h"
#include "GridsDefine.h"



namespace Grids
{
	
	class GInterface
	{
		public:
			
			GInterface( const char *, GObjectController *, GPersonController *, GMessageController * );
			
			void sendEvent( GEvent );
			
		private:
			GObjectController * object_controller;
			GPersonController * person_controller;
			GMessageController * message_controller;
			
			Protocol gprotocol;
			
			char * node_address;
			
	};

} // end namespace Grids