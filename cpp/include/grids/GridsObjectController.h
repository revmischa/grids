/*
 *  GridsObjectController.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#pragma once

#include "GridsEvent.h"
#include "GridsObject.h"

namespace Grids
{
	class GObjectController
	{
		public:
			GObjectController();
			
			addObject( GObject );
			
			sendEvent( GEvent ); // sends an event to the GInterface
			
			giveEvent( GEvent ); // The GInterface passes messages using this
			
		private:
			GInterface * interface;
			
	};
	
} // end namepace Grids
