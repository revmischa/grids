/*
 *  camera.cpp
 *  kaleidoscope
 *
 *  Created by Patrick Tierney on 8/10/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <kaleidoscope/kaleidoscope.h>
#include <kaleidoscope/camera.h>

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
		Camera(  d, MAYA );
	}
	
	Camera::Camera( Device * d, int in_type )
	{
		Camera(  d, in_type, Vec3D( 100.0f, -100.0f, 100.0f ), Vec3D( 1.0f, 0.0f, 1.0f ), Vec3D( 0.0f, 1.0f, 0.0f ) );
	}
	
	Camera::Camera( Device * d, int in_type, Vec3D position, Vec3D target, Vec3D up )
	{
		Camera( d, in_type, position, target, up, 30.0f, 0.01f );
	}
	
	Camera::Camera( Device * d, int in_type, Vec3D position, Vec3D target, Vec3D up, float rotate_speed, float translate_speed )
	{
		
		d->type = in_type;
		d->Position = position;
		d->Target = position;
		d->Target.normalize();
		d->Target *= -1.0f;
		d->UpVector = up;
		d->RotateSpeed = rotate_speed;
		d->TranslateSpeed = translate_speed;
			
		d->firstUpdate = true;
		
		d->Rotation = Vec3D(10, 10, 0);
		
		d->MAX_VERTICAL_ANGLE = 89.0f;
		
		d->ZoomSpeed = 0.04f;
		d->CenterOfRotation = Vec3D( 0.0f, 0.0f, 0.0f );
		
		d->ZoomType = ZOOM_CENTER;
		
		d->Translating = false;
		d->Rotating = false;
		d->Zooming = false;
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
		  lookAtPoint( d, d->CenterOfRotation );
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
		gluLookAt( d->Target.X, d->Target.Y, d->Target.Z, // eyex, eyey, eyez
			d->Position.X, d->Position.Y, d->Position.Z, // centerx, centery, centerz
			d->UpVector.X, d->UpVector.Y, d->UpVector.Z); 	
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
		
		doMovementMaya( d );
	}
	
	void Camera::doMovementMaya( Device * d )
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
		
		if( timeDiff == 0 )
			timeDiff = 1;
		
		SDL_Event event;
		
		SDL_PollEvent( &event );
		
		//int mouse_button = event.button.button; 
		
		if( event.button.button == 4 || event.button.button == 5 )
		{
			//std::cout << "Mouse Wheel" << std::endl;
			
			Vec3D zoomVector;
			Vec3D PositionDifference;
			Vec3D TargetDifference;
			
			if( d->ZoomType == ZOOM_FORWARD)
			{
				Vec3D normalTarget = d->Target;
				normalTarget.normalize();
				
				if( event.button.button == 4 )
				{
					zoomVector = normalTarget * ( timeDiff * d->ZoomSpeed * -1.0f  );
				}
				else
				{
					zoomVector = normalTarget * ( timeDiff * d->ZoomSpeed );
				}
				
				d->Position += zoomVector;
				d->Target += zoomVector;
				
			}
			else if( d->ZoomType == ZOOM_CENTER )
			{
				PositionDifference = d->Position - d->CenterOfRotation ;
				TargetDifference = d->Target - d->CenterOfRotation ;
				
				float scaleAmount; 
								
				if( event.button.button == 4 ) // Zoom Closer
				{
					//scaleAmount *= 1.0f;
					scaleAmount = 1.0f + timeDiff * d->ZoomSpeed * -1.0f ;
				}
				else if( event.button.button == 5 ) // Zoom Out
				{
					scaleAmount = 1.0f + timeDiff * d->ZoomSpeed ;
				}
				
				PositionDifference *= scaleAmount;
				TargetDifference *= scaleAmount;
				
				d->Position = d->CenterOfRotation + PositionDifference;
				d->Target = d->CenterOfRotation + TargetDifference;
			}
			
		}
		
		//NOTE: The mouse cursor should be hidden when the mouse is dragged
		
		//  TRANSLATION
		if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON( SDL_BUTTON_RIGHT ) ) // True when left mouse button is down
		{
			//std::cout << "Mouse Left "<< clock() << std::endl;
			
			Vec2D cursorPos = d->getCursorController()->getRelativePosition( d );
			
			if( !( d->Translating ) )
			// This allows the mouse to hover around and click on items.  Then, when the 
			// mouse is pressed, the view changes.
			{
				d->cursor_save = cursorPos;
				d->getCursorController()->setPosition( 0.5f, 0.5f, d );
				
				d->Translating = true;
				SDL_ShowCursor( 0 );
			}
			else
			{
				if(  !Equals( cursorPos.X, 0.5f) ||  !Equals( cursorPos.Y, 0.5f)  )
				{
					//std::cout << "Difference "<< clock() << std::endl;
					float offsetX = ( cursorPos.X - 0.5f ) * timeDiff;
					float offsetY = ( cursorPos.Y - 0.5f ) * timeDiff;
					
					Vec3D strafeVector = d->Target.crossProduct( d->UpVector );
					Vec3D elevationVector = d->Target.crossProduct( strafeVector );
					strafeVector.normalize();
					elevationVector.normalize();
					
					Vec3D translateHorizontle = strafeVector * (   d->TranslateSpeed * offsetX  * 1000.0f);
					Vec3D translateVertical = elevationVector * ( -1.0f * d->TranslateSpeed * offsetY * 1000.0f);
					
					Vec3D translateAmount = translateHorizontle + translateVertical;
					
					d->Position += translateAmount;
					d->Target += translateAmount;
					d->CenterOfRotation += translateAmount;
					
					d->getCursorController()->setPosition( 0.5f, 0.5f, d );
				}
			}
		}
		// ROTATION 
		else if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON( SDL_BUTTON_LEFT ) ) // True when right mouse button is down
		{
			//std::cout << "Mouse Right"<< std::endl;
			
			Vec2D cursorPos = d->getCursorController()->getRelativePosition( d );
			
			if( !( d->Rotating ) )
			{
				d->cursor_save = cursorPos;
				d->getCursorController()->setPosition( 0.5f, 0.5f, d );
				
				d->Rotating = true;
				
				SDL_ShowCursor( 0 );
			}
			else
			{
				if(  !Equals( cursorPos.X, 0.5f) ||  !Equals( cursorPos.Y, 0.5f)  )
				{
					float offsetX = (cursorPos.X - 0.5f) * timeDiff * d->RotateSpeed * 0.1f;
					float offsetY = (cursorPos.Y - 0.5f) * timeDiff * d->RotateSpeed * 0.1f;
					
					Vec3D strafeVector = d->Target.crossProduct( d->UpVector );
					Vec3D elevationVector = d->Target.crossProduct( strafeVector );
					strafeVector.normalize();
					elevationVector.normalize();
					
					d->Position -= d->CenterOfRotation;
					d->Target -= d->CenterOfRotation;
					
					d->Position = rotateAroundAxis( &(d->Position), &elevationVector, 1.0f*offsetY );
					d->Position = rotateAroundAxis( &(d->Position), &strafeVector, -1.0f*offsetX );
					
					d->Target = rotateAroundAxis( &(d->Target), &elevationVector, 1.0f*offsetY );
					d->Target = rotateAroundAxis( &(d->Target), &strafeVector, -1.0f*offsetX );
					
					d->UpVector = rotateAroundAxis( &(d->UpVector), &elevationVector, 1.0f*offsetY );
					d->UpVector = rotateAroundAxis( &(d->UpVector), &strafeVector, -1.0f*offsetX );
					
					d->Position += d->CenterOfRotation;
					d->Target += d->CenterOfRotation;
					
					d->getCursorController()->setPosition( 0.5f, 0.5f, d );
				} // end if Cursor != center
				
			} // end d->Rotating == true
		} 
		else
		{
			if( d->Translating || d->Rotating || d->Zooming )
			{
				d->getCursorController()->setPosition( d->cursor_save.X, d->cursor_save.Y, d );
			}
			
			//std::cout << "Mouse UP " << clock() << std::endl;
			d->Translating = false;
			d->Rotating = false;
			d->Zooming = false;
			// !!!! ****  Show Cursor  **** !!!!
			
			
			SDL_ShowCursor( 1 );
		} 
		
	
	}
	
	
	/////////////
	// PRIVATE
	////////////
	
	bool Camera::Equals( float a, float b )
	{
		return ( a + 0.000001f > b ) && ( a - 0.000001f < b );    
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
	
	Vec3D Camera::rotateAroundAxis( Vec3D *in_vector, Vec3D *axis, float theta )
	{
		float ux = axis->X * in_vector->X;
		float uy = axis->X * in_vector->Y;
		float uz = axis->X * in_vector->Z;
		float vx = axis->Y * in_vector->X;
		float vy = axis->Y * in_vector->Y;
		float vz = axis->Y * in_vector->Z;
		float wx = axis->Z * in_vector->X;
		float wy = axis->Z * in_vector->Y;
		float wz = axis->Z * in_vector->Z;
		
		double si = sin(theta);
		double co = cos(theta);
		
		float xx = (float) (axis->X
				* (ux + vy + wz)
				+ (in_vector->X * (axis->Y * axis->Y + axis->Z * axis->Z) - axis->X * (vy + wz))
				* co + (-wy + vz) * si);
		float yy = (float) (axis->Y
				* (ux + vy + wz)
				+ (in_vector->Y * (axis->X * axis->X + axis->Z * axis->Z) - axis->Y * (ux + wz))
				* co + (wx - uz) * si);
		float zz = (float) (axis->Z
				* (ux + vy + wz)
				+ (in_vector->Z * (axis->X * axis->X + axis->Y * axis->Y) - axis->Z * (ux + vy))
				* co + (-vx + uy) * si);
		return Vec3D(xx, yy, zz);  
	
	
	}


}

