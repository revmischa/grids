/*
 *  GridsQueue.cpp
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "Queue.h"


namespace Grids
{
	Queue::Queue( ) 
		: length( 0 ), first_node( NULL ), last_node( NULL )
	{
	
	}
	
	Queue::Queue( event initial_event )
		: length( 0 ), first_node( NULL ), last_node( NULL )
	{
		push( initial_event );
	}
	
	
	event Queue::pop( )
	{
		if( first_node == NULL )
		{
			// I dont know what to put here, I'd like it to return NULL
			// or something else signfying that the list is empty
			return event();
		}
		else
		{
			QueueNode * temp_node;
			event temp_event = first_node->getEvent();
			
			temp_node = first_node;
			first_node = first_node->next;
			
			delete temp_node;
			length--;
			
			return temp_event;
		}
	}
	
	
	void Queue::push( Event in_event )
	{
		QueueNode * temp_node;
		temp_node = new QueueNode( in_event );
	
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
	
	
	int Queue::size()
	{
		return length;
	}
	
} // end namespace Grids