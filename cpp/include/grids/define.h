
#pragma once

#include <string>
#include <map>
#include <iterator>
#include "vector3d.h"
#include "vector2d.h"
#include <json/value.h>

namespace Grids
{
	typedef std::string gridskey_t;
	typedef Json::Value gridsval_t;
	typedef std::map<gridskey_t, gridsval_t> gridsmap_t;
	typedef gridsmap_t::iterator giterator;
  
	typedef irr::core::vector3df Vec3D;
	typedef irr::core::vector2df Vec2D;
}


