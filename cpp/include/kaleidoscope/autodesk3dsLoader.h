/*
 *  autodesk3dsLoader.h
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 8/22/08.
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
#include <grids/define.h>
#include <kaleidoscope/define.h>

#include <3ds/3ds.h>


namespace Kaleidoscope
{
	class Autodesk3dsLoader
	{
		public:
			
			Autodesk3dsLoader();
			
			// Args: device, UUID, file name ( ie "test_model.3ds" )
			void load3ds( Device *, GridsID, std::string, bool );
			
			// Args: device, UUID, file name ( ie "test_model.3ds" ), color array
			void load3ds( Device *, GridsID, std::string, float *, bool );
	
	};

} // end namespace Kaleidoscope
