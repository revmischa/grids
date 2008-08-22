/*
 *  device.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <kaleidoscope/device.h>



namespace Kaleidoscope
{
	
	Device::Device( )
	{
		
	}
			
	Renderer * Device::getRenderer()
	{
		return renderer;
	}
	
	EventController * Device::getEventController()
	{
		return event_controller;
	}
	
	Camera * Device::getCamera()
	{
		return cam;
	}
	
	CursorController * Device::getCursorController()
	{
		return cursor_controller;
	}
	
	Builder * Device::getBuilder()
	{
		return builder;
	}
	
	Gui * Device::getGui( )
	{
		return gui;
	}

	void Device::setRenderer( Renderer * rnd )
	{
		renderer = rnd;
	}
	
	void Device::setEventController( EventController * evt)
	{
		event_controller = evt;
	}
	
	void Device::setCamera( Camera * cm)
	{
		cam = cm;
	}
	
	void Device::setCursorController( CursorController * crs_control)
	{
		cursor_controller = crs_control;
	}
	
	void Device::setBuilder( Builder * new_builder )
	{
		builder = new_builder;
	}
	
	void Device::setGui( Gui *  new_gui )
	{
		gui = new_gui;
	}
	
	void Device::addRoom( Room * r )
	{
		//rooms.push_back( r );
	}


}
