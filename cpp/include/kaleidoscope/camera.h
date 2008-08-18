/*
 *  camera.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#pragma once

#include <kaleidoscope/kaleidoscope.h>


#include <SDL/SDL_opengl.h>

#include <time.h>

#include <grids/vector3d.h>
#include <grids/define.h>

#include <kaleidoscope/define.h>
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/renderer.h>

namespace Kaleidoscope
{
	class CursorController;
	
	class Camera
	{
		public:
			
			Camera( Device * );
			
			// Specify camera type
			Camera( Device *, int ); 
			
			// type, position, target, up
			Camera( Device *, int, Vec3D, Vec3D, Vec3D );
			
			// type, position, target, up, rotate speed, move speed
			Camera( Device *, int, Vec3D, Vec3D, Vec3D, float, float);
			
			
			void mouseUpdate( int, int );
			void mouseUpdate( int, int, int, int );
			
			Vec3D getPosition();
			Vec3D getTarget();
			Vec3D getLook();
			Vec3D getUp();
			Vec3D getRotation();
			
			void setPosition( Vec3D );
			void setPosition( float, float, float );
			void setTarget( Device *, Vec3D );
			void setTarget( Device *, float, float, float );
			void setLook( Device *, Vec3D );
			void setLook( Device *, float, float, float );
			void setRotation( Device *, Vec3D );
			void setRotation( Device *, float, float, float );
			void lookAtPoint( Device *, Vec3D );
			void lookAtPoint( Device *, float, float, float );
			void setUp( Vec3D );
			void setUp( float, float, float );
			void setRotateSpeed( float );
			void setMoveSpeed( float );
			void setTranslateSpeed( float );
			void setZoomSpeed( float );
			void setMaxVerticalAngle( float );
			void setCameraToFPS();
			void setCameraToMaya();
			void swapCameraType(Device *);
			
			int getType(Device *);
			void setType( Device *, int );
			
			
			void doMovementFPS(Device *);
			void doMovementMaya( Device * );
			void callgluLookAt( Device *);
			
			
			
		private:
			
			//Variables stored on Device:
			
			//int type;
//			
//			Vec3D Position;
//			Vec3D Rotation;
//			Vec3D Target;
//			
//			Vec3D TargetNormal;
//			
//			Vec3D UpVector;
//			
//			float RotateSpeed;
//			float MoveSpeed;
//			float TranslateSpeed;
//			float ZoomSpeed;
//			
//			Vec3D CenterOfRotation;
//			
//			float FOV;
//			float Aspect;
//			float Near;
//			float Far;
//			
//			// forward, back, left, right, and escape
//			int keyMap[5];
//			
//			float MAX_VERTICAL_ANGLE;
//			
//			time_t LastAnimationTime;
//			Vec2D CenterCursor;
//			bool firstUpdate;
//			bool cursorPos;
			
			Device * device;
			
			Vec3D findRotationFromVector( Vec3D );
			bool Equals( float, float );
			Vec3D rotateAroundAxis( Vec3D *, Vec3D *, float theta );
	
	
	};

} // end namespace Kaleidoscope
