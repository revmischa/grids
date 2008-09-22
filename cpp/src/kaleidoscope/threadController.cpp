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
	
	int runCreateRoomDebugThreadEntryPoint( void * d )
	{
		( (Device *)d)->getInterface()->createRoomDebug();
		return 0;
	}
	
	int runDetectSelectionThreadEntryPoint( void * d )
	{
		// get Lock on d->mouse_x
		int m_x = ( (Device *)d )->down_x;
		// release d->mouse_x
		
		// get Lock on d->mouse_y
		int m_y = ( (Device *)d )->down_y;
		// release d->mouse_y
		
		( (Device *)d)->getInterface()->getObjectController()->detectSelection( (Device *)d, m_x, m_y );
		
		return 0;
	}
	
	ThreadController::ThreadController( )
	{
		
	}
	
	void ThreadController::updateVoxelThreaded( Device * d )
	{
		d->voxel_update_thread = SDL_CreateThread( runVoxelUpdateThreadEntryPoint, d );
	}
	
	void ThreadController::detectSelectionThreaded( Device * d, Vec2D mouse_coords )
	{
		d->selection_thread = SDL_CreateThread( runDetectSelectionThreadEntryPoint, d  );
	}
	
	void ThreadController::createRoomDebugThreaded( Device * d )
	{
		d->create_room_debug_thread = SDL_CreateThread( runCreateRoomDebugThreadEntryPoint, d  );
	}
	
	
	
} // end namespace Kaleidoscope
