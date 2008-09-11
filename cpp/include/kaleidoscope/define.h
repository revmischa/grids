/*
 *  define.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#pragma once

#include <SDL_ttf/SDL_ttf.h>
#include <SDL_image/SDL_image.h>
#include <SDL/SDL.h>

#include <grids/vector3d.h>
#include <grids/vector2d.h>
#include <grids/irrMath.h>
#include <kaleidoscope/matrix4.h>

#include <map>
#include <string>

//#define FPS 1
//#define MAYA 2


// Time in milliseconds of a click ( vs a drag )
#define CLICK_LENGTH 200 

namespace Kaleidoscope
{
	typedef irr::core::vector3df Vec3D;
	typedef irr::core::vector2df Vec2D;
	typedef irr::core::matrix4 Matrix4;
	
	typedef std::string GridsID;
	
	const float GRAD_PI = irr::core::RADTODEG; // 180.0f / PI
	
	const int FPS = 1;
	const int MAYA = 2;
	
	const int ROOM_OBJECT = 10;
	const int SIMPLE_OBJECT = 11;
	const int COMPLEX_OBJECT = 12;
	
	const int ZOOM_FORWARD = 1;
	const int ZOOM_CENTER = 2;
	

}
