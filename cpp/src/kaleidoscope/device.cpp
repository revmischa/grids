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
	
	Device::~Device( )
	{
		std::cout << "Deleting device" << std::endl;
		
		if( renderer )
			delete renderer;
		
		if( event_controller )
			delete event_controller;
		
		if( cam )
			delete cam;
		
		if( cursor_controller )
			delete cursor_controller;
		
		if( builder )
			delete builder;
		
		if( gui )
			delete gui;
			
		if( interface )
			delete interface;
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
	
	Grids::Interface * Device::getInterface( )
	{
		return interface;
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
	
	void Device::setInterface( Grids::Interface * new_interface )
	{
		interface = new_interface;
	}
	
	void Device::addRoom( Room * r )
	{
		//rooms.push_back( r );
	}


}
