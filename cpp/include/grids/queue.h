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

#include <grids/queueNode.h>

namespace Grids
{
	
	class Queue
	{
		public:
		
			Queue(  );
			Queue( Event );
			
			Event pop( );
			void push( Event );
			
			int size();
			
		private:
		
			QueueNode * first_node;
			QueueNode * last_node;
			
			int length;
	
	};
}
