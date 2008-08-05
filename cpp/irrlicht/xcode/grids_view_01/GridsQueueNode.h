/*
 *  GridsQueueNode.h
 *  test_01
 *
 *  Created by Patrick Tierney on 8/5/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#pragma once

#include "GridsEvent.h"

namespace Grids
{
	
	class GQueueNode
	{
		public:
			
			
			GEvent getEvent();
		
		private:
			GQueueNode * next;
	
	};
}
