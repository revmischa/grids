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
	int runCreateRoomDebugThreadEntryPoint( void * d );
	int runDetectSelectionThreadEntryPoint( void * d );
	
	class ThreadController
	{
		public:
			ThreadController();
			
			void updateVoxelThreaded( Device * );
			void detectSelectionThreaded( Device *, Vec2D );
		
			void createRoomDebugThreaded( Device * );
	};
	
	
}// end namespace Kaleidoscope
