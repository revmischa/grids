/*
 *  kaleidoscope.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */



#pragma once

#include <kaleidoscope/device.h>
#include <kaleidoscope/renderer.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/cursorController.h>
#include <iostream>


namespace Kaleidoscope
{
	class Camera;
	class CursorController;
	class EventController;
	class Renderer;

	void createDevice( Device * new_device, int width, int height )
	{
		new_device->setCursorController( new Kaleidoscope::CursorController( new_device ) );
		new_device->setRenderer( new Kaleidoscope::Renderer( new_device, width, height) ) ;
		new_device->setEventController( new Kaleidoscope::EventController( ) );
		new_device->setCamera( new Kaleidoscope::Camera( new_device ) );
		
		new_device->getCursorController()->setDevice( new_device );
		new_device->getRenderer()->setDevice( new_device );
		new_device->getEventController()->setDevice( new_device );
		new_device->getCamera()->setDevice( new_device );

		
		std::cout << "Created Device" << std::endl;
	}

} // end namespace Kaleidoscope