/*
 *  threadController.cpp
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 9/13/08.
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
	
	void ThreadController::detectSelectionThreaded( Device * d )
	{
		d->selection_thread = SDL_CreateThread( runDetectSelectionThreadEntryPoint, d  );
	}
	
	void ThreadController::createRoomDebugThreaded( Device * d )
	{
		d->create_room_debug_thread = SDL_CreateThread( runCreateRoomDebugThreadEntryPoint, d  );
	}
	
	void ThreadController::lock( Device * d )
	{
		SDL_mutexP( d->thread_controller_mutex );
	}
	
	void ThreadController::unlock( Device * d )
	{
		SDL_mutexV( d->thread_controller_mutex );
	}
	
	
	
} // end namespace Kaleidoscope
