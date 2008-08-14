/*
 *  GridsQueueNode.h
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#pragma once

#include <grids/event.h>

namespace Grids
{
	
	class QueueNode
	{
		public:
			QueueNode( );
			QueueNode( Event );
			QueueNode( Event, QueueNode * );
			
			void setNext( QueueNode * );
			
			Event getEvent();
			
			QueueNode * next;

		private:
			Event stored_event;
	
	};
}
