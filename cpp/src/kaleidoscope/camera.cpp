/*
 *  camera.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <kaleidoscope/kaleidoscope.h>
#include "kaleidoscope/camera.h"

#include <SDL/SDL_opengl.h>

#include <SDL/SDL.h>
#include <kaleidoscope/define.h> // for vector3df lameness 

#include <iostream>
#include <math.h>
#include <time.h> // for clock

#include <SDL/SDL_opengl.h> // Attempting to remove gluLookAt error



namespace Kaleidoscope
{
	
	Camera::Camera(Device * d )
	{
		Camera(  d, FPS );
	}
	
	Camera::Camera( Device * d, int in_type )
	{
		Camera(  d, in_type, Vec3D( 10.0f, 0.0f, 10.0f ), Vec3D( 1.0f, 0.0f, 1.0f ), Vec3D( 0.0f, 1.0f, 0.0f ) );
	}
	
	Camera::Camera( Device * d, int in_type, Vec3D position, Vec3D target, Vec3D up )
	{
		Camera( d, in_type, position, target, up, 30.0f, 0.01f );
	}
	
	Camera::Camera( Device * d, int in_type, Vec3D position, Vec3D target, Vec3D up, float rotate_speed, float translate_speed )
	{
		
		d->type = in_type;
		d->Position = position;
		d->Target = target;
		d->UpVector = up;
		d->RotateSpeed = rotate_speed;
		d->TranslateSpeed = translate_speed;
			
		d->firstUpdate = true;
		
		d->Rotation = Vec3D(10, 10, 0);
		
		d->MAX_VERTICAL_ANGLE = 89.0f;
	}
	
	Vec3D Camera::getPosition()
	{
		return device->Position;
	}
	
	Vec3D Camera::getTarget()
	{
		return device->Target;
	}
	
	Vec3D Camera::getLook()
	{
		return device->Target - device->Position;
	}
	
	Vec3D Camera::getUp()
	{
		return device->UpVector;
	}
	
	Vec3D Camera::getRotation()
	{
		return device->Rotation;
	}
	
	void Camera::setPosition( Vec3D vec )
	{
		device->Position.set( vec );
	}
	
	void Camera::setPosition( float x, float y, float z)
	{
		setPosition( Vec3D( x, y, z ) );
	}
	
	void Camera::setTarget( Device * d, Vec3D vec )
	{
		d->Target.set( vec );
	}
	
	void Camera::setTarget(Device * d, float x, float y, float z )
	{
		setTarget( d, Vec3D( x, y, z ) );
	}
	
	void Camera::setLook( Device * d, Vec3D vec )
	{
		if( getType( d ) == MAYA )
		{
			d->Target = d->Position + vec ;
		}
		else if( getType(d ) == FPS )
		{
			setRotation( d, findRotationFromVector( vec ) );
		}
	}
	
	void Camera::setLook( Device * d, float x, float y, float z )
	{
		setLook( d, Vec3D( x, y, z ) );
	}
	
	void Camera::setRotation( Device * d, Vec3D vec )
	{
		d->Rotation.set( vec );
	}
	
	void Camera::setRotation(  Device * d, float x, float y, float z  )
	{
		setRotation( d, Vec3D( x, y, z ) );
	}
	
	void Camera::lookAtPoint( Device * d,  Vec3D vec )
	{
		setLook( d, vec - d->Position );
	}
	
	void Camera::lookAtPoint( Device * d,  float x, float y, float z )
	{
		lookAtPoint( d, Vec3D( x, y, z ) );
	}
	
	void Camera::setRotateSpeed( float rot )
	{
		device->RotateSpeed = rot;
	}
	
	void Camera::setMoveSpeed( float move )
	{
		device->MoveSpeed = move ;
	}
	
	void Camera::setTranslateSpeed( float trans )
	{
		device->TranslateSpeed = trans;
	}
	
	void Camera::setZoomSpeed( float zoom )
	{
		device->ZoomSpeed = zoom;
	}
	
	void Camera::setMaxVerticalAngle( float newAngle )
	{
		device->MAX_VERTICAL_ANGLE = newAngle;
	}
	
	void Camera::setCameraToFPS()
	{
		device->type = FPS;
	}
	
	void Camera::setCameraToMaya()
	{
		device->type = MAYA;
	}
	
	void Camera::swapCameraType( Device * d)
	{
		if( getType( d ) == FPS )
		{
		  setType( d, MAYA );
		}
		else if ( getType( d ) == MAYA )
		{
		  setType( d, FPS );
		  d->UpVector.set( 0.0f, 1.0f, 0.0f);
		} 
	}
	
	int Camera::getType(Device * d)
	{
		return d->type;
	}
	
	void Camera::setType( Device * d, int in_type )
	{
		d->type = in_type;
	}
	
	void Camera::callgluLookAt( Device * d)
	// This is where the magic happens
	{ // Removed for happiland
		////gluLookAt( d->Target.X, d->Target.Y, d->Target.Z, // eyex, eyey, eyez
			//d->Position.X, d->Position.Y, d->Position.Z, // centerx, centery, centerz
			//d->UpVector.X, d->UpVector.Y, d->UpVector.Z); 	
	}
	
	void Camera::doMovementFPS( Device * d)
	{

if( d->firstUpdate )
    {
        if( d->getCursorController() )
        {
            d->getCursorController()->setPosition( 0.5f, 0.5f, d );
        }
        
        d->LastAnimationTime = clock();
        d->firstUpdate = false;
    }
		
		int now = clock(); // get the current time
		int timeDiff =  now - d->LastAnimationTime;
		d->LastAnimationTime = now;
				
		if( timeDiff == 0 ) // if framerate > 1000 fps you cannot move...
			timeDiff = 1;
			
		Vec3D temp_rotation = Vec3D( d->Rotation );
		
		temp_rotation.X *= -1.0f;
		temp_rotation.Y *= -1.0f;
		
		d->Target.set( 0.0f, 0.0f, 1.0f );
		
		Vec2D cursorpos = d->getCursorController()->getRelativePosition( d );
		
		// if mouseX != lastMouseX, though this is called in the 
		// GLUT event mouseMoved, so we can skip the step
		
		if(   !Equals( cursorpos.X, 0.5f) ||  !Equals( cursorpos.Y, 0.5f)  )
		{
			temp_rotation.Y += ( 0.5f - cursorpos.X) * d->RotateSpeed * timeDiff;
			temp_rotation.X -= ( 0.5f - cursorpos.Y) * d->RotateSpeed * timeDiff;
						
			d->getCursorController()->setPosition( 0.5f, 0.5f, d );
			
			if( temp_rotation.X > d->MAX_VERTICAL_ANGLE) temp_rotation.X = d->MAX_VERTICAL_ANGLE;
			if( temp_rotation.X < -( d->MAX_VERTICAL_ANGLE ) ) temp_rotation.X = -( d->MAX_VERTICAL_ANGLE );
		}
			
		Matrix4 mat = Matrix4();
		
		mat.setRotationDegrees( Vec3D( -temp_rotation.X, temp_rotation.Y, 0.0f ) );
		
		irr::core::vector3df temp_irr_target = irr::core::vector3df( d->Target );
		mat.transformVect( temp_irr_target );
		
		temp_irr_target.normalize();
		
		d->Target.set( temp_irr_target );
		
		Uint8* keys;		
		
		keys = SDL_GetKeyState( NULL );
		
		if( keys[SDLK_UP] )
		{
			d->Position -= d->Target * ( timeDiff * d->TranslateSpeed );				
		}
		else if( keys[SDLK_DOWN] )
		{
			d->Position += d->Target * ( timeDiff * d->TranslateSpeed );
		}
			
		Vec3D strafevect = Vec3D( d->Target );
		strafevect = strafevect.crossProduct( d->UpVector );
		strafevect.normalize();
			
		if( keys[SDLK_LEFT] )
		{
			d->Position += strafevect * ( timeDiff * d->TranslateSpeed );
		}
		else if(  keys[SDLK_RIGHT] )
		{
			d->Position -= strafevect * ( timeDiff * d->TranslateSpeed );	
		}
		
		d->Target += d->Position;
		
		temp_rotation.X *= -1.0f;
		temp_rotation.Y *= -1.0f;
		
		d->Rotation = Vec3D( temp_rotation );
		
		d->TargetNormal = ( d->Position - d->Target ).normalize();
		
	}
	
	
	
	
	
	
	/////////////
	// PRIVATE
	////////////
	
	bool Camera::Equals( float a, float b )
	{
		return ( a + 0.001f > b ) && ( a - 0.001f < b );    
	}
	
	Vec3D Camera::findRotationFromVector( Vec3D VecIn )
	{
		Vec3D angle = Vec3D();
		
		Vec3D VecIn2 = Vec3D( VecIn );
		
		VecIn2.invert();
		
		angle.Y = atan2( VecIn2.X, VecIn2.Z );
		angle.Y *= GRAD_PI;
		
		angle.Y *= -1.0f;
		
		float z1;
		z1 = sqrt( VecIn2.X * VecIn2.X + VecIn2.Z * VecIn2.Z );
		
		angle.X = atan2( z1, VecIn2.Y );
		angle.X *= GRAD_PI;
		angle.X -= 90.0f;
		
		return angle;
	}


}

