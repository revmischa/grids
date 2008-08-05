/*
 *  GridsQueue.h
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *	
 *	GQueue is a linked list of events
 *	
 *
 */

#include "GridsQueueNode.h"

namespace Grids
{
	
	class GQueue
	{
		public:
			GQueue(  );
			GQueue( GQueueNode );
			
			GEvent pop( );
			
		private:
			void push( GQueueNode );
			
			GQueueNode *first_node;
	
	};
}