/*
 *  camera.h
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *
 *	 This file is part of Grids/Kaleidoscope.
 *	 
 *	 Grids/Kaleidoscope is free software: you can redistribute it and/or modify
 *	 it under the terms of the GNU General Public License as published by
 *	 the Free Software Foundation, either version 3 of the License, or
 *	 (at your option) any later version.
 *	 
 *	 Grids/Kaleidoscope is distributed in the hope that it will be useful,
 *	 but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	 GNU General Public License for more details.
 *	 
 *	 You should have received a copy of the GNU General Public License
 *	 along with Grids/Kaleidoscope.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once


#include <kaleidoscope/define.h>
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/renderer.h>

#include <grids/vector3d.h>
#include <grids/define.h>

#include <SDL/SDL_opengl.h>

#include <time.h>

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
			
			Vec3D getPosition( Device *);
			Vec3D getTarget( Device *);
			Vec3D getLook( Device *);
			Vec3D getUp( Device *);
			Vec3D getRotation( Device *);
			
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
			void setPerspective( Device *, float, float, float, float );
			
			int getType(Device *);
			void setType( Device *, int );
			
			
			void doMovementFPS(Device *);
			void doMovementMaya( Device * );
			void callgluLookAt( Device *);
		
			void getRayFromScreenCoordinates( Device *, Vec2D, float * );
			
			
			
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
