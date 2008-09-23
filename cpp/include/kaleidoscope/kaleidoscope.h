/*
 *  kaleidoscope.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
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

#include <stdlib.h>    // For malloc() etc.
#include <stdio.h>     // For printf(), fopen() etc.
#include <math.h>      // For sin(), cos() etc.

#include <kaleidoscope/device.h>
#include <kaleidoscope/renderer.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/RenderObject.h>
#include <kaleidoscope/room.h>
#include <kaleidoscope/builder.h>
#include <kaleidoscope/autodesk3dsLoader.h>


#include <iostream>

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

namespace Kaleidoscope
{
	class Camera;
	class CursorController;
	class EventController;
	class Renderer;
	class Device;
	class Builder;
	class Autodesk3dsLoader;

	//void createDevice( Device * new_device, int width, int height )
//	{
//		new_device->setCursorController( new Kaleidoscope::CursorController( new_device ) );
//		new_device->setRenderer( new Kaleidoscope::Renderer( new_device, width, height) ) ;
//		new_device->setEventController( new Kaleidoscope::EventController( new_device ) );
//		new_device->setCamera( new Kaleidoscope::Camera( new_device ) );
//		
//		std::cout << "Created Device" << std::endl;
//	}

} // end namespace Kaleidoscope
