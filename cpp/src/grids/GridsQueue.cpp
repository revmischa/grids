/*
 *  GridsQueue.cpp
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "GridsQueue.h"


namespace Grids
{
	GQueue::GQueue( ) 
		: length( 0 ), first_node( NULL ), last_node( NULL )
	{
	
	}
	
	GQueue::GQueue( GEvent initial_event )
		: length( 0 ), first_node( NULL ), last_node( NULL )
	{
		push( initial_event );
	}
	
	
	GEvent GQueue::pop( )
	{
		if( first_node == NULL )
		{
			// I dont know what to put here, I'd like it to return NULL
			// or something else signfying that the list is empty
			return GEvent();
		}
		else
		{
			GQueueNode * temp_node;
			GEvent temp_event = first_node->getEvent();
			
			temp_node = first_node;
			first_node = first_node->next;
			
			delete temp_node;
			length--;
			
			return temp_event;
		}
	}
	
	
	void GQueue::push( GEvent in_event )
	{
		GQueueNode * temp_node;
		temp_node = new GQueueNode( in_event );
	
		if( first_node == NULL )
		{
			first_node = temp_node;
			last_node = temp_node;
		}
		else
		{
			last_node->next = temp_node;
			last_node = temp_node;
		}
		
		length++;
	}
	
	
	int GQueue::size()
	{
		return length;
	}
	
} // end namespace Grids