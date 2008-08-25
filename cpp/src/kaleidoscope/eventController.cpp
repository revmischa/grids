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

		if( keys[SDLK_SPACE]  )
		{
			d->Position = Vec3D ( 20.0f, 20.0f, 20.0f );
			d->getCamera()->lookAtPoint( d, 0.0f, 0.0f, 0.0f );
		}

		if( keys[SDLK_ESCAPE] )
		{
			d->running = 0;
		}

		if( keys[SDLK_TAB] )
		{
			if( SDL_GetTicks() - d->last_clock > 400 )
			{
				d->getCamera()->swapCameraType( d );
				d->last_clock = SDL_GetTicks();
			}
		}

		if( keys[SDLK_6] )
		{
			if( SDL_GetTicks() - d->last_clock > 400 ) // 10 milliseconds
			{
				//d->interface->createRoom( );
				d->last_clock = SDL_GetTicks();
			}
		}

		if( keys[SDLK_7] )
		{
			if( SDL_GetTicks() - d->last_clock > 400 )
			{
				Grids::Value object_type;

				object_type[ "roomId" ] = d->world_hash[ "rooms" ][ 0u ];

				object_type[ "position" ][ 0u ] = 0.0f;
				object_type[ "position" ][ 1u ] = 0.0f;
				object_type[ "position" ][ 2u ] = 0.0f;
				object_type[ "scale" ][ 0u ] = 1.0f;
				object_type[ "scale" ][ 1u ] = 1.0f;
				object_type[ "scale" ][ 2u ] = 1.0f;
				object_type[ "rotation" ][ 0u ] = 0.0f;
				object_type[ "rotation" ][ 1u ] = 0.0f;
				object_type[ "rotation" ][ 2u ] = 0.0f;

				d->getInterface()->sendEvent( "Object.Place", object_type );

				d->last_clock = SDL_GetTicks();
			}
		}

		if( keys[SDLK_8] )
		{
			if( SDL_GetTicks() - d->last_clock > 400 )
			{
				std::cout << "Key Pressed, Attempting to create room" << std::endl;

				d->getInterface()->createRoom();

				d->last_clock = SDL_GetTicks();

			}
		}

		if( keys[96] ) // ~ key, used to open console
		{
			if( SDL_GetTicks() - d->last_clock > 400 ) // 100 milliseconds
			{
				std::cout << "Escape" << std::endl;


				d->last_clock = SDL_GetTicks();
			}
		}




		// If the mouse is clicked ( not dragged )
		// pass that information on to the gui

		///////////////////////////
		////  CLICK vs DRAG DETECTION
		/////////////////////////

		if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON( SDL_BUTTON_LEFT ) ) // True when left mouse button is down
		{
			if( d->left_pressed == false )
			{
				d->left_pressed = true;

				d->mouse_timer = SDL_GetTicks();
			}
		}
		else
		{
			if( d->left_pressed && ( SDL_GetTicks() - d->mouse_timer ) < CLICK_LENGTH )
			// A click must be under 2/5 th of a second
			//
			{
				int x, y;
				SDL_GetMouseState( &x, &y );

				// Alert GUI and pass on the location of the click
				d->getGui()->registerClick( SDL_BUTTON_LEFT, x, y );
			}

			d->left_pressed = false;
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
