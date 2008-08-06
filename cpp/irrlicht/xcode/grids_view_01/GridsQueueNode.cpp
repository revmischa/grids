/*
 *  GridsQueueNode.cpp
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "GridsQueueNode.h"


namespace Grids
{
	
	GQueueNode::GQueueNode( )
	{
	
	}
	
	GQueueNode::GQueueNode( GEvent in_event )
		: stored_event( in_event )
	{
	
	}
	
	GQueueNode::GQueueNode( GEvent in_event, GQueueNode * next_node )
		: stored_event( in_event ), next( next_node )
	{
	
	}
	
	void GQueueNode::setNext( GQueueNode * next_node )
	{
		next = next_node;
	}
	
	GEvent GQueueNode::getEvent( )
	// Returns the stored event of the queue node
	{
		return stored_event;
	}
	
	
}