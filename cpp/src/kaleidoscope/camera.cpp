/*
 *  camera.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include "camera.h"
#include <math.h>
#include <OPENGL/glu.h>
#include <kaleidoscope/define.h> // for vector3df lameness 
#include <iostream>
#include <pthread.h>

namespace Kaleidoscope
{
	
	Camera::Camera(Device * dvc )
	{
		Camera(  dvc, FPS );
	}
	
	Camera::Camera( Device * dvc, int in_type )
	{
		Camera(  dvc, in_type, Vec3D( 10.0f, 0.0f, 10.0f ), Vec3D( 1.0f, 0.0f, 1.0f ), Vec3D( 0.0f, 1.0f, 0.0f ) );
	}
	
	Camera::Camera( Device * dvc, int in_type, Vec3D position, Vec3D target, Vec3D up )
	{
		Camera( dvc, in_type, position, target, up, 20.0f, 0.04f );
	}
	
	Camera::Camera( Device * dvc, int in_type, Vec3D position, Vec3D target, Vec3D up, float rotate_speed, float translate_speed )
	{
		device = dvc;
		
		device->type = in_type;
		device->Position = position;
		device->Target = target;
		device->UpVector = up;
		device->RotateSpeed = rotate_speed;
		device->TranslateSpeed = translate_speed;
			
		device->firstUpdate = true;
		
		device->Rotation = Vec3D(10, 10, 0);
		
		device->MAX_VERTICAL_ANGLE = 89.0f;
	}
	
	void Camera::setDevice( Device * dvc )
	{
		device = dvc;
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
	
	void Camera::setTarget( Vec3D vec )
	{
		device->Target.set( vec );
	}
	
	void Camera::setTarget( float x, float y, float z )
	{
		setTarget( Vec3D( x, y, z ) );
	}
	
	void Camera::setLook( Vec3D vec )
	{
		if( getType() == MAYA )
		{
			device->Target = device->Position + vec ;
		}
		else if( getType() == FPS )
		{
			setRotation( findRotationFromVector( vec ) );
		}
	}
	
	void Camera::setLook( float x, float y, float z )
	{
		setLook( Vec3D( x, y, z ) );
	}
	
	void Camera::setRotation( Vec3D vec )
	{
		device->Rotation.set( vec );
	}
	
	void Camera::setRotation(  float x, float y, float z  )
	{
		setRotation( Vec3D( x, y, z ) );
	}
	
	void Camera::lookAtPoint( Vec3D vec )
	{
		setLook( vec - getPosition() );
	}
	
	void Camera::lookAtPoint( float x, float y, float z )
	{
		lookAtPoint( Vec3D( x, y, z ) );
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
	
	void Camera::swapCameraType()
	{
		if( getType() == FPS )
		{
		  setType( MAYA );
		}
		else if ( getType() == MAYA )
		{
		  setType( FPS );
		  device->UpVector.set( 0.0f, 1.0f, 0.0f);
		} 
	}
	
	int Camera::getType()
	{
		return device->type;
	}
	
	void Camera::setType( int in_type )
	{
		device->type = in_type;
	}
		
	
	
	void Camera::callgluLookAt()
	// This is where the magic happens
	{
		gluLookAt( device->Target.X, device->Target.Y, device->Target.Z, // eyex, eyey, eyez
			device->Position.X, device->Position.Y, device->Position.Z, // centerx, centery, centerz
			device->UpVector.X, device->UpVector.Y, device->UpVector.Z); 	
	}
	
	void Camera::doMovementFPS( Device * d)
	// this is called on mouseMoved
	{
		
		if( d->firstUpdate )
		{
			if( d->getCursorController() )
			{
				d->getCursorController()->setPosition( 0.5f, 0.5f );
				d->CenterCursor = Vec2D( 0.5f, 0.5f );
			}
			
			d->LastAnimationTime = glutGet(GLUT_ELAPSED_TIME);
			d->firstUpdate = false;
		}
		
		d->getCursorController()->setDevice( d );
				
		int now = glutGet(GLUT_ELAPSED_TIME); // get the current time
		int timeDiff =  now - d->LastAnimationTime;
		d->LastAnimationTime = now;
				
		if( timeDiff == 0 ) // if framerate > 1000 fps you cannot move...
			timeDiff = 1;
			
		Vec3D temp_rotation = Vec3D( device->Rotation );
		
		
		temp_rotation.X *= -1.0f;
		temp_rotation.Y *= -1.0f;
		
		setTarget( 0.0f, 0.0f, 1.0f );
		
		Vec2D cursorpos = d->getCursorController()->getRelativePosition( d );
		
		//std::cout << "cur  "<<  cursorpos.X << " : " << cursorpos.Y <<std::endl;

		
		// if mouseX != lastMouseX, though this is called in the 
		// GLUT event mouseMoved, so we can skip the step
		
		if(  !Equals( cursorpos.X, 0.5f) || !Equals( cursorpos.Y, 0.5f )  )
		{
			temp_rotation.Y += ( 0.5f - cursorpos.X) * d->RotateSpeed * timeDiff;
			temp_rotation.X -= ( 0.5f - cursorpos.Y) * d->RotateSpeed * timeDiff;
			
			// std::cout << "FUCK " <<  cursorpos.X <<std::endl;
			//std::cout << "cur  "<<  cursorpos.X << " : " << cursorpos.Y <<std::endl;
						
			device->getCursorController()->setPosition( 0.5f, 0.5f );
			
			if( temp_rotation.X > d->MAX_VERTICAL_ANGLE) temp_rotation.X = d->MAX_VERTICAL_ANGLE;
			if( temp_rotation.X < -( d->MAX_VERTICAL_ANGLE ) ) temp_rotation.X = -( d->MAX_VERTICAL_ANGLE );
		}
			
		Matrix4 mat = Matrix4();
		
		mat.setRotationDegrees( Vec3D( -temp_rotation.X, temp_rotation.Y, 0.0f ) );
		
		irr::core::vector3df temp_irr_target = irr::core::vector3df( d->Target );
		mat.transformVect( temp_irr_target );
		d->Target.set( temp_irr_target );
		
		d->Target.normalize();
		
		//std::cout << Target.X << " : " << Target.Y << " : " << Target.Z << std::endl;
		
		if( d->getEventController()->keyPressed() )
		{
			int special_key = d->getEventController()->specialKey();
			
			if( special_key == GLUT_KEY_UP )
			{
				d->Position -= d->Target * ( timeDiff * d->TranslateSpeed );				
			}
			else if( special_key == GLUT_KEY_DOWN )
			{
				d->Position += d->Target * ( timeDiff * d->TranslateSpeed );
			}
			
			Vec3D strafevect = Vec3D( d->Target );
			strafevect = strafevect.crossProduct( d->UpVector );
			strafevect.normalize();
			
			if( special_key == GLUT_KEY_LEFT )
			{
				d->Position += strafevect * ( timeDiff * d->TranslateSpeed );
				//std::cout << "Left: " << Position.X << " : " << Position.Y << " : " << Position.Z << std::endl;
			}
			else if( special_key == GLUT_KEY_RIGHT )
			{
				d->Position -= strafevect * ( timeDiff * d->TranslateSpeed );	
				//std::cout << "Right: " << Position.X << " : " << Position.Y << " : " << Position.Z << std::endl;		
			}
		
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
		return ( a + 0.0001f > b ) && ( a - 0.0001f < b );    
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