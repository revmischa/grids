/*
 *  GridsQueueNode.h
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#pragma once

#include "GridsEvent.h"

namespace Grids
{
	
	class GQueueNode
	{
		public:
			GQueueNode( );
			GQueueNode( GEvent );
			GQueueNode( GEvent, GQueueNode * );
			
			void setNext( GQueueNode * );
			
			GEvent getEvent();
		
		private:
			GQueueNode * next;
			GEvent stored_event;
	
	};
}
