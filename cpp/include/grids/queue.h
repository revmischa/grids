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

#pragma once 

#include "GridsQueueNode.h"

namespace Grids
{
	
	class GQueue
	{
		public:
		
			GQueue(  );
			GQueue( GEvent );
			
			GEvent pop( );
			void push( GEvent );
			
			int size();
			
		private:
		
			GQueueNode * first_node;
			GQueueNode * last_node;
			
			int length;
	
	};
}
