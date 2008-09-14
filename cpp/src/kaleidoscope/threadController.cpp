/*
 *  threadController.cpp
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 9/13/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <kaleidoscope/threadController.h>


namespace Kaleidoscope 
{
	
	int runVoxelUpdateThreadEntryPoint( void * d )
	{
		( (Device *)d)->getVoxel()->update( (Device *)d, 3, 0.45f);
		return 0;
	}
	
	ThreadController::ThreadController( )
	{
		
	}
	
	void ThreadController::updateVoxelThreaded( Device * d )
	{
		d->voxel_update_thread = SDL_CreateThread( runVoxelUpdateThreadEntryPoint, d );
	}
	
	
	
} // end namespace Kaleidoscope
