/*
 *  device.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */



#pragma once

#include <kaleidoscope/renderer.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/room.h>
#include <kaleidoscope/RenderObject.h>

#include <vector>
#include <map>


namespace Kaleidoscope
{
	class Renderer;
	class Camera;
	class CursorController;
	class EventController;
	class Room;
	class RenderObject;
	
	class Device
	{
		public:
			
			Device( );
			
			Renderer * getRenderer();
			EventController * getEventController();
			Camera * getCamera();
			CursorController * getCursorController();
			
			void setRenderer( Renderer * );
			void setEventController( EventController * );
			void setCamera( Camera * );
			void setCursorController( CursorController * );
			
			std::vector< Room * > rooms;
						
			std::map< Room *, std::map< RenderObject *, std::vector< float > > > vertex_hash;
			
			// Room ID =>  < Object => quad_vectices ... Object => quad_vertices  ....  Object => quad_vertices  >
			std::map< std::string, std::map< std::string, std::vector< float > > > room_objects_hash;
			
			// ID => Type    eg room, person, etc
			std::map< std::string, int > id_type_hash;
			
			// Type => All Objects of Type
			std::map< int, std::vector< void * > > type_objects_hash;
			
			// ID => pointer stored in memory
			std::map< std::string, void * > id_object_hash;
			
			void addRoom( Room * );
			
			
			int running;
			
			
			///////////
			// Camera Variables
			//////////
				
			int type;
			
			Vec3D Position;
			Vec3D Rotation;
			Vec3D Target;
			
			Vec3D TargetNormal;
			
			Vec3D UpVector;
			
			float RotateSpeed;
			float MoveSpeed;
			float TranslateSpeed;
			float ZoomSpeed;
			
			Vec3D CenterOfRotation;
			
			float FOV;
			float Aspect;
			float Near;
			float Far;
			
			// forward, back, left, right, and escape
			int keyMap[5];
			
			float MAX_VERTICAL_ANGLE;
			
			int LastAnimationTime;
			Vec2D CenterCursor;
			bool firstUpdate;
			bool cursorPos;
			
			
			/////////////////
			//Rendere Variables
			////////////////
			
			
			int width;
			int height;
			int x_pos; // stores the window's upper left x position
			int y_pos; // stores the window's upper right y position
			
			int current_text_mode;
			char *text_mode_string[4];
			GLint text_modes[4];
			
			int texture_id;
			int window_id;
						
			bool Texture_On;
			bool Light_On;
			bool Alpha_Add;
			bool Blend_On;
			bool Filtering_On;
			bool Smooth_On;
			
			float Light_Ambient[4];
			float Light_Diffuse[4];
			float Light_Position[4];
			
			
			//////////////
			// Event Variables
			/////////////
			
			int mouseX;
			int mouseY;
			
			unsigned char stored_key;
			int special_key;
			
			int mouseButton;
			int mouseState;
			
			bool leftPressed;
			bool rightPressed;
			bool middlePressed;
			
			bool key_pressed;
			bool special_key_pressed;
			
			/////////////////
			//  Cursor Variables
			///////////////
			
			int last_x;
			int last_y;
			
			
			
			
						
		private:
			
			Renderer * renderer;
			EventController * event_controller;
			Camera * cam;
			CursorController * cursor_controller;
			
			
			
			
			
	};

} // end namespace Kaleidoscope
