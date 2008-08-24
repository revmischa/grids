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
#include <kaleidoscope/builder.h>
#include <kaleidoscope/gui.h>
#include <kaleidoscope/RenderObject.h>

#include <grids/interface.h>

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
	class Builder;
	class Gui;
	
	class Device
	{
		public:
			
			Device( );
			~Device();
			
			Renderer * getRenderer();
			EventController * getEventController();
			Camera * getCamera();
			CursorController * getCursorController();
			Builder * getBuilder();
			Gui * getGui();
			Grids::Interface * getInterface( );
						
			void setRenderer( Renderer * );
			void setEventController( EventController * );
			void setCamera( Camera * );
			void setCursorController( CursorController * );
			void setBuilder( Builder * );
			void setGui( Gui * );
			void setInterface( Grids::Interface * );
			
			
			// ******  MAKE THIS A POINTER	
			
			Grids::complex_type  world_hash;
			
			void addRoom( Room * );
			
			int running;
			
			int last_clock;
			
			float room_width;
			
			
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
			int ZoomType;
			
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
			
			bool Translating;
			bool Rotating;
			bool Zooming;
			
			Vec2D cursor_save;
			
			
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
			
			bool left_pressed;
			bool right_pressed;
			
			bool key_pressed;
			bool special_key_pressed;
			
			int mouse_timer;
			
			
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
			Builder * builder;
			Gui * gui;
			Grids::Interface * interface;
			
			
			
			
			
	};

} // end namespace Kaleidoscope
