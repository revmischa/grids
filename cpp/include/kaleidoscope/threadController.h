/*
 *  threadController.h
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
			void detectSelectionThreaded( Device * );
		
			void createRoomDebugThreaded( Device * );
	};
	
	
}// end namespace Kaleidoscope
