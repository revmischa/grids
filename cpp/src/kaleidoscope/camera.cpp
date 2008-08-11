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


namespace Kaleidoscope
{
	
	Camera::Camera(Device * dvc )
	{
		Camera(  dvc, FPS );
	}
	
	Camera::Camera( Device * dvc, int in_type )
	{
		Camera(  dvc, in_type, Vec3D( 0.0f, 0.0f, 0.0f ), Vec3D( 1.0f, 0.0f, 0.0f ), Vec3D( 0.0f, 1.0f, 0.0f ) );
	}
	
	Camera::Camera( Device * dvc, int in_type, Vec3D position, Vec3D target, Vec3D up )
	{
		Camera( dvc, in_type, position, target, up, 1.0, 1.0 );
	}
	
	Camera::Camera( Device * dvc, int in_type, Vec3D position, Vec3D target, Vec3D up, float rotate_speed, float translate_speed )
		:	device( dvc ), type( in_type ), Position( position ), Target( target ), UpVector( up ), 
			RotateSpeed( rotate_speed), TranslateSpeed( translate_speed)
	{
		if( device )
		{
			std::cout << "It worked" << std::endl;
			if( device->getCursorController() )
			{
				std::cout << "It worked" << std::endl;

			}
		}
			
		
		LastAnimationTime = time( NULL );
		firstUpdate = true;
	
	}
	
	void Camera::setDevice( Device * dvc )
	{
		device = dvc;
	}
	
	Vec3D Camera::getPosition()
	{
		return Position;
	}
	
	Vec3D Camera::getTarget()
	{
		return Target;
	}
	
	Vec3D Camera::getLook()
	{
		return Target - Position;
	}
	
	Vec3D Camera::getUp()
	{
		return UpVector;
	}
	
	Vec3D Camera::getRotation()
	{
		return Rotation;
	}
	
	
	void Camera::setPosition( Vec3D vec )
	{
		Position.set( vec );
	}
	
	void Camera::setPosition( float x, float y, float z)
	{
		setPosition( Vec3D( x, y, z ) );
	}
	
	void Camera::setTarget( Vec3D vec )
	{
		Target.set( vec );
	}
	
	void Camera::setTarget( float x, float y, float z )
	{
		setTarget( Vec3D( x, y, z ) );
	}
	
	void Camera::setLook( Vec3D vec )
	{
		if( type == MAYA )
		{
			Target = Position + vec ;
		}
		else if( type == FPS )
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
		Rotation.set( vec );
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
		RotateSpeed = rot;
	}
	
	void Camera::setMoveSpeed( float move )
	{
		MoveSpeed = move ;
	}
	
	void Camera::setTranslateSpeed( float trans )
	{
		TranslateSpeed = trans;
	}
	
	void Camera::setZoomSpeed( float zoom )
	{
		ZoomSpeed = zoom;
	}
	
	void Camera::setMaxVerticalAngle( float newAngle )
	{
		MAX_VERTICAL_ANGLE = newAngle;
	}
	
	void Camera::setCameraToFPS()
	{
		type = FPS;
	}
	
	void Camera::setCameraToMaya()
	{
		type = MAYA;
	}
	
	void Camera::swapCameraType()
	{
		if( type == FPS )
		{
		  type = MAYA;
		}
		else if ( type == MAYA )
		{
		  type = FPS;
		  UpVector.set( 0.0f, 1.0f, 0.0f);
		} 
	}
	
	void Camera::callgluLookAt()
	// This is where the magic happens
	{
		gluLookAt( Target.X, Target.Y, Target.Z, // eyex, eyey, eyez
			Position.X, Position.Y, Position.Z, // centerx, centery, centerz
			UpVector.X, UpVector.Y, UpVector.Z); 	
	}
	
	void Camera::doMovementFPS()
	// this is called on mouseMoved
	{
		
		if( firstUpdate)
		{
			if( device->getCursorController() )
			{
				device->getCursorController()->setPosition( 0.5f, 0.5f );
				CenterCursor = device->getCursorController()->getRelativePosition();
			}
			
			LastAnimationTime = time( NULL);
			firstUpdate = false;
		}
		
				
		time_t now = time( NULL ); // get the current time
		double timeDiff = difftime( now, LastAnimationTime );
		LastAnimationTime = now;
		
		Rotation.X *= -1.0f;
		Rotation.Y *= -1.0f;
		
		Target.set( 0.0f, 0.0f, 1.0f );
		
		Vec2D cursorpos = device->getCursorController()->getRelativePosition();
		
		// if mouseX != lastMouseX, though this is called in the 
		// GLUT event mouseMoved, so we can skip the step
		
		if( cursorpos != CenterCursor )
		{
			Rotation.Y += ( 0.5f - cursorpos.X) * RotateSpeed * timeDiff;
			Rotation.X += ( 0.5f - cursorpos.Y) * RotateSpeed * timeDiff;
			
			device->getCursorController()->setPosition( 0.5f, 0.5f );
			
			if( Rotation.X > MAX_VERTICAL_ANGLE) Rotation.X = MAX_VERTICAL_ANGLE;
			if( Rotation.Y < -MAX_VERTICAL_ANGLE) Rotation.X = -MAX_VERTICAL_ANGLE;
		}
			
		Matrix4 mat = Matrix4();
		
		mat.setRotationDegrees( Vec3D( -Rotation.X, Rotation.Y, 0.0f ) );
		
		float temp_array[3];
		mat.transformVect( &temp_array[0],  Target );
		Target.set( temp_array[0], temp_array[1], temp_array[2] );
		Target.normalize();
		
		if( device->getEventController()->keyPressed() )
		{
			int special_key = device->getEventController()->specialKey();
			
			if( special_key == GLUT_KEY_UP )
			{
				Position -= Target * ( timeDiff * MoveSpeed );
			
			}
			else if( special_key == GLUT_KEY_DOWN )
			{
				Position += Target * ( timeDiff * MoveSpeed );
			}
			
			Vec3D strafevect = Vec3D( Target );
			strafevect = strafevect.crossProduct( UpVector );
			strafevect.normalize();
			
			if( special_key == GLUT_KEY_LEFT )
			{
				Position += strafevect * ( timeDiff * MoveSpeed );
			}
			else if( special_key == GLUT_KEY_RIGHT )
			{
				Position -= strafevect * ( timeDiff * MoveSpeed );			
			}
		
		}
		
		Target += Position;
		Rotation.X *= -1.0f;
		Rotation.Y *= -1.0f;
		
		TargetNormal = ( Position - Target ).normalize();
		
	}
	
	
	
	
	
	
	/////////////
	// PRIVATE
	////////////
	
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