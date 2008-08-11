/*
 *  eventController.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "eventController.h"


namespace Kaleidoscope
{
	
	EventController::EventController( Device * in_device )
	{
		device = in_device;
		
		device->key_pressed = false;
	}
	
	void EventController::setDevice( Device * in_device)
	{
		device = in_device ;
	}
	
	void EventController::mousePressedCall( int button, int state, int x, int y )
	{
		device->mouseButton = button;
		device->mouseState = state;
		
		device->mouseX = x;
		device->mouseY = y;
	}
	
	void EventController::mouseMovedCall( int x, int y )
	{
		device->mouseX = x;
		device->mouseY = y;
	}
	
	
	void EventController::mouseDraggedCall( int x, int y )
	{
		device->mouseX = x;
		device->mouseY = y;
		
	}
	
	void EventController::keyPressedCall( unsigned char in_key, int x, int y)
	// x and y store the mouse coordinates for some reason
	{
		device->key_pressed = true;
		device->stored_key = in_key;
		
		if( in_key == 49 )
		{
			device->Texture_On = !device->Texture_On;
		}
		else if( in_key == 50 )
		{
			device->Light_On = !device->Light_On;
		}
		else if( in_key == 51 )
		{
			device->Alpha_Add = !device->Alpha_Add;
		}
		else if( in_key == 52 )
		{
			device->Blend_On = !device->Blend_On;
		}
		else if( in_key == 53 )
		{
			device->Filtering_On = !device->Filtering_On;
		}
	}
	
	void EventController::specialKeyPressedCall( int in_key, int x, int y )
	// x and y store the mouse coordinates for some reason
	{
		device->special_key_pressed = true;
		device->special_key = in_key;
	}
	
	void EventController::keyUpCall( unsigned char in_key, int x, int y )
	{
		device->key_pressed = false;
	}
	
	void EventController::specialKeyUpCall( int in_key, int x, int y )
	{
		device->special_key_pressed = false;
	}
	
	int EventController::getMouseX()
	{
		return device->mouseX;
	}
	
	int EventController::getMouseY()
	{
		return device->mouseY;
	}
	
	unsigned char EventController::key()
	{
		return device->stored_key;
	}
	
	int EventController::specialKey()
	{
		return device->special_key;
	}
	
	bool EventController::keyPressed()
	{
		return device->key_pressed;
	}
	
	bool EventController::specialKeyPressed()
	{
		return device->special_key_pressed;
	}


} // end namespace Kaleidoscope