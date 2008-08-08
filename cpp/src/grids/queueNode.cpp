/*
 *  GridsQueueNode.cpp
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "queueNode.h"


namespace Grids
{
	
	QueueNode::QueueNode( )
		: next( NULL )
	{
	
	}
	
	QueueNode::QueueNode( Event in_event )
		: stored_event( in_event ), next( NULL )
	{
	
	}
	
	QueueNode::QueueNode( Event in_event, QueueNode * next_node )
		: stored_event( in_event ), next( next_node )
	{
	
	}
	
	void QueueNode::setNext( QueueNode * next_node )
	{
		next = next_node;
	}
	
	Event QueueNode::getEvent( )
	// Returns the stored event of the queue node
	{
		return stored_event;
	}
	
	
}