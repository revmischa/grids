/*
 *  eventController.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <kaleidoscope/eventController.h>

#include <SDL/SDL.h>
#include <grids/define.h> // for Vec3D
#include <iostream>

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
	
	void EventController::checkEvents( Device * d )
	{
		
		Uint8* keys;		
		
		keys = SDL_GetKeyState( NULL );
		
		if(  keys[SDLK_1] )
		{
			d->Texture_On = !(d->Texture_On);
			std::cout << "Texture: " << d->Texture_On << std::endl;
		}
		
		if( keys[SDLK_2] )
		{
			d->Light_On = !(d->Light_On);
			std::cout << "Light: " << d->Light_On << std::endl;
		}
		
		if( keys[SDLK_3] )
		{
			d->Alpha_Add = !(d->Alpha_Add);
			std::cout << "Alpha: " << d->Alpha_Add << std::endl;
		}
		
		if( keys[SDLK_4] )
		{
			d->Blend_On = !(d->Blend_On);
			std::cout << "Blend: " << d->Blend_On << std::endl;
		}
		
		if( keys[SDLK_5] )
		{
			d->Filtering_On = !(d->Filtering_On );
			std::cout << "Filternig: " << d->Filtering_On << std::endl;
		}
		
		if( keys[SDLK_SPACE] )
		{
			d->Position = Vec3D ( 20.0f, 20.0f, 20.0f );
			d->getCamera()->lookAtPoint( d, 0.0f, 0.0f, 0.0f );
		}
		
		if( keys[SDLK_ESCAPE] )
		{
			d->running = 0;
		}
		
		/*
		
		if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_LEFT ) )
		{
			d->leftPressed = true;
		}
		else
		{
			d->leftPressed = false;
		}
		
		if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_RIGHT ) )
		{
			d->rightPressed = true;
		}
		else
		{
			d->rightPressed = false;
		}
		
		if( glfwGetMouseButton( GLFW_MOUSE_BUTTON_MIDDLE ) )
		{
			d->middlePressed = true;
		}
		else
		{
			d->middlePressed = false;
		}
		
		d->mouseWheelPosition = glfwGet
		*/
			
	
	
	
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
