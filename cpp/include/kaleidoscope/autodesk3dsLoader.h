/*
 *  autodesk3dsLoader.h
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 8/22/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
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
