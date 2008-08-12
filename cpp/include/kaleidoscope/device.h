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


namespace Kaleidoscope
{
	class Renderer;
	class Camera;
	class CursorController;
	class EventController;
	
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
