/*
 *  define.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#pragma once

#include <grids/vector3d.h>
#include <grids/vector2d.h>
#include <grids/irrMath.h>
#include <kaleidoscope/matrix4.h>

//#define FPS 1
//#define MAYA 2

namespace Kaleidoscope
{
	typedef irr::core::vector3df Vec3D;
	typedef irr::core::vector2df Vec2D;
	typedef irr::core::matrix4 Matrix4;
	
	const float GRAD_PI = irr::core::RADTODEG; // 180.0f / PI
	
	const int FPS = 1;
	const int MAYA = 2;

}
