
#pragma once

#include <string>
#include <map>
#include <iterator>
#include "vector3d.h"

namespace Grids
{


  /*  typedef const char * gridskey_t;
      typedef const char * gridsval_t;*/

	typedef std::string gridskey_t;
	typedef std::string gridsval_t;
	typedef std::map<gridskey_t, gridsval_t> gridsmap_t;
	typedef gridsmap_t::iterator giterator;
  
	typedef irr::core::vector3df Vec3D;
}


