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
	
	EventController::EventController( )
	{
		key_pressed = false;
	}
	
	void EventController::mousePressedCall( int button, int state, int x, int y )
	{
		mouseButton = button;
		mouseState = state;
	}
	
	void EventController::mouseMovedCall( int x, int y )
	{
		mouseX = x;
		mouseY = y;
	}
	
	void EventController::keyPressedCall( unsigned char in_key, int x, int y)
	// x and y store the mouse coordinates for some reason
	{
		key_pressed = true;
		stored_key = in_key;
	}
	
	void EventController::specialKeyPressedCall( int in_key, int x, int y )
	// x and y store the mouse coordinates for some reason
	{
		key_pressed = true;
		special_key = in_key;
	}
	
	void EventController::keyUpCall( unsigned char in_key, int x, int y )
	{
		key_pressed = false;
	}
	
	void EventController::specialKeyUpCall( int in_key, int x, int y )
	{
		key_pressed = false;
	}
	
	int EventController::getMouseX()
	{
		return mouseX;
	}
	
	int EventController::getMouseY()
	{
		return mouseY;
	}
	
	unsigned char EventController::key()
	{
		return stored_key;
	}
	
	int EventController::specialKey()
	{
		return special_key;
	}
	
	bool EventController::keyPressed()
	{
		return key_pressed;
	}


} // end namespace Kaleidoscope