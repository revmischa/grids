/*
 *  GridsQueueNode.cpp
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

#include <grids/queueNode.h>


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
