/*
 *  threadController.h
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 9/13/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#pragma once

#include <kaleidoscope/device.h>

namespace Kaleidoscope
{
	int runVoxelUpdateThreadEntryPoint( void * d );
	
	class ThreadController
	{
		public:
			ThreadController();
			
			void updateVoxelThreaded( Device * );
	};
	
	
}// end namespace Kaleidoscope
