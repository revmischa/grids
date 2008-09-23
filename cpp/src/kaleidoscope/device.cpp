/*
 *  device.cpp
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
			
		if( loader )
			delete loader;
			
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
	
	Autodesk3dsLoader * Device::getLoader()
	{
		return loader;
	}
	
	VoxelSpace * Device::getVoxel()
	{
		return voxel;
	}
	
	ThreadController * Device::getThreadController()
	{
		return thread_controller;
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
	
	void Device::setLoader( Autodesk3dsLoader * new_loader )
	{
		loader = new_loader;
	}
	
	void Device::setVoxel( VoxelSpace * new_voxel )
	{
		voxel = new_voxel;
	}
	
	void Device::setThreadController( ThreadController * new_tc )
	{
		thread_controller = new_tc;
	}
	
	
	void Device::addRoom( Room * r )
	{
		//rooms.push_back( r );
	}


}
