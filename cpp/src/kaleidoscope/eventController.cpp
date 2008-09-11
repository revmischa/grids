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
#include <kaleidoscope/define.h>

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

		std::string temp_string;
		
		SDL_Event event;
		
		d->mouseButton = 0;
		d->mouse_moved = false;
		
		while( SDL_PollEvent( &event ) )
		{
			if( event.type == SDL_KEYDOWN )
			{
				if( event.key.keysym.sym == SDLK_TAB )
				{
					d->getCamera()->swapCameraType( d );
				}
				else if( event.key.keysym.sym == SDLK_1 )
				{
					d->Texture_On = !(d->Texture_On);
				}
				else if( event.key.keysym.sym == SDLK_2 )
				{
					d->Light_On = !(d->Light_On);
				}
				else if( event.key.keysym.sym == SDLK_3 )
				{
					d->Alpha_Add = !(d->Alpha_Add);
				}
				else if( event.key.keysym.sym == SDLK_4 )
				{
					d->Blend_On = !(d->Blend_On);
				}
				else if( event.key.keysym.sym == SDLK_5 )
				{
					d->Filtering_On = !(d->Filtering_On );
				}
				else if( event.key.keysym.sym == SDLK_ESCAPE )
				{
					d->running = 0;
				}
				else if( ( event.key.keysym.sym > 96 && event.key.keysym.sym <= 122 ) || event.key.keysym.sym == SDLK_SPACE )
				{
					temp_string = "";
					temp_string += (char)event.key.keysym.sym;
					d->getGui()->appendText( d, 1u, 0u, temp_string);
					
					// NOTE: I still need to add support for SHIFT and CAPS LOCK keys
				}
				else if( event.key.keysym.sym == SDLK_RETURN )
				{
					d->getGui()->parseText( d, 1u, 0u );
					//std::cout << "return" << std::endl;
				}
				else if( event.key.keysym.sym == SDLK_BACKSPACE )
				{
					d->getGui()->removeLastCharacter( d, 1u, 0u );
				}
				else if( event.key.keysym.sym == SDLK_DELETE )
				{
					d->getGui()->clearText( d, 1u, 0u );
				}
			}
			
			///////////////////////////
			////  CLICK vs DRAG DETECTION
			/////////////////////////
			
			else if( event.type == SDL_MOUSEBUTTONDOWN )
			{
				// record the first state
				int x, y;
				SDL_GetMouseState( &x, &y );
				
				d->down_x = x;
				d->down_y = y;
								
				d->mouseButton = event.button.button;
				d->mouse_down = true;
			}
			else if( event.type == SDL_MOUSEBUTTONUP )
			{
				// if the first click is the same as up then
				int x, y;
				SDL_GetMouseState( &x, &y );
				
				if(		d->down_x == x && d->down_y == y && 
						event.button.button != 4 && event.button.button != 5	)
				{
					d->getGui()->registerClick( SDL_BUTTON_LEFT, x, y );
				}
				
				d->mouse_down = false;
			}
			else if( event.type == SDL_MOUSEMOTION )
			{
				d->mouse_moved = true;
			}
			else if( event.type == SDL_QUIT )
			{
				d->running = 0;
			}
		}


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
