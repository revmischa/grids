/*
 *  device.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */



#pragma once

#include <kaleidoscope/renderer.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/cursorController.h>


namespace Kaleidoscope
{
	class Renderer;
	class Camera;
	class CursorController;
	
	class Device
	{
		public:
			
			Device( );
			
			Renderer * getRenderer();
			EventController * getEventController();
			Camera * getCamera();
			CursorController * getCursorController();
			
			void setRenderer( Renderer * );
			void setEventController( EventController * );
			void setCamera( Camera * );
			void setCursorController( CursorController * );
			
		private:
			
			Renderer * renderer;
			EventController * event_controller;
			Camera * cam;
			CursorController * cursor_controller;
			
	};

} // end namespace Kaleidoscope
