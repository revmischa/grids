
#pragma once

#include <string>
#include <map>
#include <iterator>

#include <grids/vector3d.h>
#include <grids/vector2d.h>

#include <grids/value.h>

#define GRIDS_ID_ERROR "ID ERROR"
#define GRIDS_CREATE_ROOM "Room.Create" 
#define GRIDS_CREATE_OBJECT "Room.CreateObject"
#define GRIDS_UPDATE_OBJECT "Room.UpdateObject"


namespace Grids {
	typedef irr::core::vector3df Vec3D;
	typedef irr::core::vector2df Vec2D;

	typedef std::string GridsID;

}


