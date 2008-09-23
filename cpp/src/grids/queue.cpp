/*
 *  GridsQueue.cpp
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *
 *	 This file is part of Grids/Kaleidoscope.
 *	 
 *	 Grids/Kaleidoscope is free software: you can redistribute it and/or modify
 *	 it under the terms of the GNU General Public License as published by
 *	 the Free Software Foundation, either version 3 of the License, or
 *	 (at your option) any later version.
 *	 
 *	 Grids/Kaleidoscope is distributed in the hope that it will be useful,
 *	 but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	 GNU General Public License for more details.
 *	 
 *	 You should have received a copy of the GNU General Public License
 *	 along with Grids/Kaleidoscope.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <grids/queue.h>


namespace Grids
{
	Queue::Queue( ) 
		: length( 0 ), first_node( NULL ), last_node( NULL )
	{
	
	}
	
	Queue::Queue( Event initial_event )
		: length( 0 ), first_node( NULL ), last_node( NULL )
	{
		push( initial_event );
	}
	
	
	Event Queue::pop( )
	{
		if( first_node == NULL )
		{
			// I dont know what to put here, I'd like it to return NULL
			// or something else signfying that the list is empty
			return Event();
		}
		else
		{
			QueueNode * temp_node;
			Event temp_event = first_node->getEvent();
			
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
