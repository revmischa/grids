/*
 *  camera.cpp
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

#include <kaleidoscope/camera.h>
#include <kaleidoscope/define.h> // for vector3df lameness 

#include <SDL/SDL_opengl.h>
#include <SDL/SDL.h>

#include <iostream>

#include <math.h>
#include <time.h> // for clock


namespace Kaleidoscope
{
	
	Camera::Camera(Device * d )
	{
		Camera(  d, FPS ); // BUG!!!  If the camera starts at MAYA there are GUI bugs.
							//		camera renders wrong sizes, orientations.
	}
	
	Camera::Camera( Device * d, int in_type )
	{
		Camera(  d, in_type, Vec3D( 10.0f, -10.0f, 10.0f ), Vec3D( -1.0f, -1.0f, -1.0f ), Vec3D( 0.0f, 1.0f, 0.0f ) );
	}
	
	Camera::Camera( Device * d, int in_type, Vec3D position, Vec3D target, Vec3D up )
	{
		Camera( d, in_type, position, target, up, 30.0f, 0.01f );
	}
	
	Camera::Camera( Device * d, int in_type, Vec3D position, Vec3D target, Vec3D up, float rotate_speed, float translate_speed )
	{
		lock( d );
		
		d->type = in_type;
		d->Position = position;
		
		target.normalize();
		d->Target = position + target;
		d->Target.normalize();
		d->Target *= -1.0f;
		
		d->UpVector = up;
		d->RotateSpeed = rotate_speed;
		d->TranslateSpeed = translate_speed;
		d->MoveSpeed = 1.0f;
			
		d->firstUpdate = true;
		
		d->Rotation = Vec3D(10, -10, 0);
		
		d->MAX_VERTICAL_ANGLE = 89.0f;
		
		d->ZoomSpeed = 0.14f;
		d->CenterOfRotation = Vec3D( 0.0f, 0.0f, 0.0f );
		
		d->ZoomType = ZOOM_CENTER;
		
		d->Translating = false;
		d->Rotating = false;
		d->Zooming = false;
		
		unlock( d );
	}
	
	Vec3D Camera::getPosition( Device * d )
	{
		Vec3D temp_pos;
		
		lock( d );
		temp_pos = d->Position;
		unlock( d );
		
		return temp_pos;
	}
	
	Vec3D Camera::getTarget( Device * d)
	{
		Vec3D temp_tar;
		
		lock( d );
		temp_tar = d->Target;
		unlock( d );
		
		return temp_tar;
	}
	
	Vec3D Camera::getLook( Device * d)
	{
		Vec3D temp_look;
		
		lock( d );
		temp_look = d->Target - d->Position;
		unlock( d );
		
		return temp_look;
	}
	
	Vec3D Camera::getUp( Device * d)
	{
		Vec3D temp_up;
		
		lock( d );
		temp_up = d->UpVector;
		unlock( d );
		
		return temp_up;
	}
	
	Vec3D Camera::getRotation( Device * d)
	{
		Vec3D temp_rot;
		
		lock( d );
		temp_rot = d->Rotation;
		unlock( d );
		
		return temp_rot;
	}
	
	void Camera::setPosition( Device * d, Vec3D vec )
	{
		lock( d );
		d->Position.set( vec );
		unlock( d );
	}
	
	void Camera::setPosition( Device * d, float x, float y, float z)
	{
		setPosition( d, Vec3D( x, y, z ) );
	}
	
	void Camera::setTarget( Device * d, Vec3D vec )
	{
		lock( d );
		d->Target.set( vec );
		unlock( d );
	}
	
	void Camera::setTarget(Device * d, float x, float y, float z )
	{
		setTarget( d, Vec3D( x, y, z ) );
	}
	
	void Camera::setUp( Device * d, Vec3D vec )
	{
		lock( d );
		d->UpVector.set( vec );
		unlock( d );
	}
	
	void Camera::setUp(Device * d, float x, float y, float z )
	{
		setUp( d, Vec3D( x, y, z ) );
	}
	
	
	void Camera::setLook( Device * d, Vec3D vec )
	{
		if( getType( d ) == MAYA )
		{
			vec.normalize();
			
			lock( d );
			d->Target = d->Position + vec ;
			unlock( d );
			
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
		lock( d );
		d->Rotation.set( vec );
		unlock( d );
	}
	
	void Camera::setRotation(  Device * d, float x, float y, float z  )
	{
		setRotation( d, Vec3D( x, y, z ) );
	}
	
	void Camera::setPerspective( Device * d, float fovy, float aspect, float zNear, float zFar )
	{
		lock( d );
		d->Fov = fovy;
		d->Aspect = aspect;
		d->Near = zNear;
		d->Far = zFar;
		unlock( d );
		
		gluPerspective( fovy, aspect, zNear, zFar );
	}
	
	void Camera::getRayFromScreenCoordinates( Device * d, Vec2D coords, float * screen_ray )
	{		
		Vec3D farLeftUp, farRightUp, farLeftDown;
		Vec3D nearLeftUp, nearRightUp, nearLeftDown;
		Vec3D tarVec, upVec, posVec;
		
		float temp_fov, temp_near, temp_far, temp_aspect;
		int temp_width, temp_height;
		
		lock( d );
		
		temp_fov = d->Fov;
		temp_near = d->Near;
		temp_far = d->Far;
		temp_aspect = d->Aspect;
		posVec = d->Position;
		tarVec = d->Target - d->Position;
		upVec = d->UpVector;
		temp_width = d->width;
		temp_height = d->height;
		
		unlock( d );
		
		float Hnear =  tan(temp_fov / 2) * temp_near;
		float Wnear = Hnear * temp_aspect;
		
		float Hfar =  tan(temp_fov / 2) * temp_far;
		float Wfar = Hfar * temp_aspect;
				
		tarVec.normalize();
		
		Vec3D strafeVec = tarVec.crossProduct( upVec );
		strafeVec.normalize();
		
		Vec3D elVec = tarVec.crossProduct( strafeVec );
		elVec.normalize();
		
		Vec3D farCenter = posVec + ( tarVec * temp_far ); 
		
		farLeftUp = farCenter + ( elVec * ( Hfar / 2.0f ) )  - ( strafeVec * ( Wfar / 2.0f ) );      
		
		farRightUp = farCenter + ( elVec * ( Hfar / 2.0f ) )  + ( strafeVec * ( Wfar / 2.0f ) );
		
		farLeftDown = farCenter - ( elVec * ( Hfar / 2.0f ) )  - ( strafeVec * ( Wfar / 2.0f ) ); 
		
		
		Vec3D nearCenter = posVec + ( tarVec * temp_near ); 
		
		nearLeftUp = nearCenter + ( elVec * ( Hnear / 2.0f ) )  - ( strafeVec * ( Wnear / 2.0f ) );      
		
		nearRightUp = nearCenter + ( elVec * ( Hnear / 2.0f ) )  + ( strafeVec * ( Wnear / 2.0f ) );
		
		nearLeftDown = nearCenter - ( elVec * ( Hnear / 2.0f ) )  - ( strafeVec * ( Wnear / 2.0f ) );
		
		
		Vec3D lefttorightFar =  farRightUp - farLeftUp ;
		Vec3D uptodownFar = farLeftDown - farLeftUp ;
		
		
		Vec3D lefttorightNear =  nearRightUp - nearLeftUp ;
		Vec3D uptodownNear = nearLeftDown - nearLeftUp ;
		
		
		float dx = ( float )( coords.X ) / (float)( temp_width );
		float dy = ( float )( coords.Y ) / (float)( temp_height );   
		
		Vec3D directionFar =  farLeftUp + ( lefttorightFar * dx )  + ( uptodownFar * dy );
		Vec3D directionNear =  nearLeftUp + ( lefttorightNear * dx )  + ( uptodownNear * dy );
		
		Vec3D screen_ray_temp = directionNear - directionFar;
		
		screen_ray_temp.normalize();
		
		screen_ray[ 0 ] = directionNear.X;
		screen_ray[ 1 ] = directionNear.Y;
		screen_ray[ 2 ] = directionNear.Z;
		
		screen_ray[ 3 ] = screen_ray_temp.X;
		screen_ray[ 4 ] = screen_ray_temp.Y;
		screen_ray[ 5 ] = screen_ray_temp.Z;
			
	}
	
	void Camera::lookAtPoint( Device * d,  Vec3D vec )
	{
		Vec3D posVec;
		
		lock( d );
		posVec = d->Position;
		unlock( d );
		
		setLook( d, vec - posVec );
	}
	
	void Camera::lookAtPoint( Device * d,  float x, float y, float z )
	{
		lookAtPoint( d, Vec3D( x, y, z ) );
	}
	
	void Camera::setRotateSpeed( Device * d, float rot )
	{
		lock( d );
		d->RotateSpeed = rot;
		unlock( d );
	}
	
	void Camera::setMoveSpeed( Device * d, float move )
	{
		lock( d );
		d->MoveSpeed = move ;
		unlock( d );
	}
	
	void Camera::setTranslateSpeed( Device * d, float trans )
	{
		lock( d );
		d->TranslateSpeed = trans;
		unlock( d );
	}
	
	void Camera::setZoomSpeed( Device * d, float zoom )
	{
		lock( d );
		d->ZoomSpeed = zoom;
		unlock( d );
	}
	
	void Camera::setMaxVerticalAngle( Device * d, float newAngle )
	{
		lock( d );
		d->MAX_VERTICAL_ANGLE = newAngle;
		unlock( d );
	}
	
	void Camera::setCameraToFPS( Device * d )
	{
		lock( d );
		d->type = FPS;
		unlock( d );
	}
	
	void Camera::setCameraToMaya( Device * d )
	{
		lock( d );
		d->type = MAYA;
		unlock( d );
	}
	
	void Camera::swapCameraType( Device * d )
	{
		if( getType( d ) == FPS )
		{
			// Attempting to set the center rotation to a point 30 units away from the 
			// camera based on targetr
			lock( d );
			
			Vec3D new_center =  d->Target - d->Position ;
			new_center.normalize();
			new_center *= -35.0f ;
			d->CenterOfRotation = d->Position + new_center;
			
			setType( d, MAYA );
			//lookAtPoint( d, d->CenterOfRotation );
			
			unlock( d );
		}
		else if ( getType( d ) == MAYA )
		{
			lock( d );
			
			std::cout << "Changed Camera from MAYA" << std::endl;
			setType( d, FPS );
			d->UpVector.set( 0.0f, 1.0f, 0.0f);
			lookAtPoint( d, d->CenterOfRotation );
			SDL_ShowCursor( 0 );
			
			unlock( d );
		}
	}
	
	int Camera::getType(Device * d)
	{
		int temp_type;
		
		lock( d );
		temp_type = d->type;
		unlock( d );
		
		return temp_type;
	}
	
	void Camera::setType( Device * d, int in_type )
	{
		lock( d );
		d->type = in_type;
		unlock( d );
	}
	
	void Camera::callgluLookAt( Device * d)
	// This is where the magic happens
	{ 
		lock( d );
		gluLookAt( d->Target.X, d->Target.Y, d->Target.Z, // eyex, eyey, eyez
			d->Position.X, d->Position.Y, d->Position.Z, // centerx, centery, centerz
			d->UpVector.X, d->UpVector.Y, d->UpVector.Z); 	
		unlock( d );
		
//		std::cout << getPosition( d ).X << " : " << getPosition( d ).Y << " : " << getPosition( d ).Z << std::endl;
//		std::cout << getTarget( d ).X << " : " << getTarget( d ).Y << " : " << getTarget( d ).Z << std::endl;
//		std::cout << getUp( d ).X << " : " << getUp( d ).Y << " : " << getUp( d ).Z << std::endl;

	}
	
	void Camera::doMovementFPS( Device * d)
	{
		lock( d );
		if( d->firstUpdate )
		{
			if( d->getCursorController() )
			{
				d->getCursorController()->setPosition( 0.5f, 0.5f, d );
			}
			
			d->LastAnimationTime = SDL_GetTicks();
			d->firstUpdate = false;
		}
		
		int now = SDL_GetTicks(); // get the current time
		int timeDiff =  now - d->LastAnimationTime;
		d->LastAnimationTime = now;
				
		if( true || timeDiff == 0  ) // if framerate > 1000 fps you cannot move...
			timeDiff = 1;
			
		Vec3D temp_rotation = Vec3D( d->Rotation );
		
		temp_rotation.X *= -1.0f;
		temp_rotation.Y *= -1.0f;
		
		d->Target.set( 0.0f, 0.0f, 1.0f );
		
		Vec2D cursorpos = d->getCursorController()->getRelativePosition( d );
		
		if(  !Equals( cursorpos.X, 0.5f) ||  !Equals( cursorpos.Y, 0.5f)  )
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
			d->Position -= d->Target * ( timeDiff * d->MoveSpeed );				
		}
		else if( keys[SDLK_DOWN] )
		{
			d->Position += d->Target * ( timeDiff * d->MoveSpeed );
		}
			
		Vec3D strafevect = Vec3D( d->Target );
		strafevect = strafevect.crossProduct( d->UpVector );
		strafevect.normalize();
			
		if( keys[SDLK_LEFT] )
		{
			d->Position += strafevect * ( timeDiff * d->MoveSpeed );
		}
		else if(  keys[SDLK_RIGHT] )
		{
			d->Position -= strafevect * ( timeDiff * d->MoveSpeed );	
		}
		
		d->Target += d->Position;
		
		temp_rotation.X *= -1.0f;
		temp_rotation.Y *= -1.0f;
		
		d->Rotation = Vec3D( temp_rotation );
		
		d->TargetNormal = ( d->Position - d->Target ).normalize();
	
		unlock( d );
		
	} // end doMovementFPS
	
	
	
	void Camera::doMovementMaya( Device * d )
	{
		lock( d );
		
		if( d->firstUpdate )
		{
			if( d->getCursorController() )
			{
				d->getCursorController()->setPosition( 0.5f, 0.5f, d );
			}
			
			d->LastAnimationTime = SDL_GetTicks();
			d->firstUpdate = false;
		}
		
		int now = SDL_GetTicks(); // get the current time
		int timeDiff =  now - d->LastAnimationTime;
		d->LastAnimationTime = now;
		
		if( true || timeDiff == 0 )
			timeDiff = 1;
		
		if( ( d->mouseButton ==  4 || d->mouseButton ==  5 )  &&  !( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON( SDL_BUTTON_RIGHT ) ) )
		{
			//std::cout << "Mouse Wheel" << std::endl;
			
			Vec3D zoomVector;
			Vec3D PositionDifference;
			Vec3D TargetDifference;
			
			if( d->ZoomType == ZOOM_FORWARD)
			{
				Vec3D normalTarget = d->Target;
				normalTarget.normalize();
				
				if( d->mouseButton ==  4 )
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
								
				if(  d->mouseButton ==  4 ) // Zoom Closer
				{
					scaleAmount = 1.0f + timeDiff * d->ZoomSpeed * -1.0f ;
				}
				else if( d->mouseButton ==  5  ) // Zoom Out
				{
					scaleAmount = 1.0f + timeDiff * d->ZoomSpeed ;
				}
				
				PositionDifference *= scaleAmount;
				TargetDifference *= scaleAmount;
				
				d->Position = d->CenterOfRotation + PositionDifference;
				d->Target = d->CenterOfRotation + TargetDifference;
				
				Vec3D temp_tar =  d->Target - d->Position; // The target must be normalized for selection / cursor-ray finding to work
				temp_tar.normalize();
				d->Target = d->Position + temp_tar;
			}
			
		}
		
		// SDL poll event
		
		//NOTE: The mouse cursor should be hidden when the mouse is dragged
		
		if( d->mouse_moved )
		{
		
			//  TRANSLATION
			if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON( SDL_BUTTON_RIGHT ) ) // True when left mouse button is down
			{
				
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
						//std::cout << "Difference "<< SDL_GetTicks() << std::endl;
						float offsetX = ( cursorPos.X - 0.5f ) * timeDiff;
						float offsetY = ( cursorPos.Y - 0.5f ) * timeDiff;
						
						//Vec3D look = d->Target - d->Position;
						
						Vec3D strafeVector = d->Target.crossProduct( d->UpVector );
						Vec3D elevationVector = d->Target.crossProduct( strafeVector );
						strafeVector.normalize();
						elevationVector.normalize();
						
						// Implamented so that you pan faster when you are farther away from the room, etc
						float dist_to_center = d->Position.getDistanceFrom( d->CenterOfRotation );
						
						Vec3D translateHorizontle = strafeVector * (   d->TranslateSpeed * offsetX  * 1000.0f * dist_to_center / 50.0f );
						Vec3D translateVertical = elevationVector * ( -1.0f * d->TranslateSpeed * offsetY * 1000.0f * dist_to_center / 50.0f );
						
						Vec3D translateAmount = translateHorizontle + translateVertical;
						
						d->Position += translateAmount;
						d->Target += translateAmount;
						d->CenterOfRotation += translateAmount;
						
						d->getCursorController()->setPosition( 0.5f, 0.5f, d );
					}
				}
			}
			// ROTATION 
			else if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON( SDL_BUTTON_LEFT ) && true ) // True when left mouse button is down --// now - d->mouse_timer > CLICK_LENGTH
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
						float offsetX = (cursorPos.Y - 0.5f) * timeDiff * d->RotateSpeed * -0.1f ;
						float offsetY = (cursorPos.X - 0.5f) * timeDiff * d->RotateSpeed * -0.1f ;
						
						Vec3D strafeVector = d->Target.crossProduct( d->UpVector );
						Vec3D elevationVector = d->Target.crossProduct( strafeVector );
						strafeVector.normalize();
						elevationVector.normalize();
						
						d->Position -= d->CenterOfRotation;
						d->Target -= d->CenterOfRotation;
						
						d->Position = rotateAroundAxis( &(d->Position), &elevationVector, -1.0f*offsetY );
						d->Position = rotateAroundAxis( &(d->Position), &strafeVector, -1.0f*offsetX );
						
						d->Target = rotateAroundAxis( &(d->Target), &elevationVector, -1.0f*offsetY );
						d->Target = rotateAroundAxis( &(d->Target), &strafeVector, -1.0f*offsetX );
						
						// FOR WACKY ROTATION, ROTATE THE UP VECTOR
						// LEAVING IT UNTOUCHED 
						//d->UpVector = rotateAroundAxis( &(d->UpVector), &elevationVector, -1.0f*offsetY );
						//d->UpVector = rotateAroundAxis( &(d->UpVector), &strafeVector, -1.0f*offsetX );
						
						d->Position += d->CenterOfRotation;
						d->Target += d->CenterOfRotation;
						
						d->getCursorController()->setPosition( 0.5f, 0.5f, d );
					} // end if Cursor != center
					
				} // end d->Rotating == true
			} 
		}
		else
		{
			if( d->mouse_down == false )
			{	
				if( d->Translating || d->Rotating || d->Zooming )
				{
					d->getCursorController()->setPosition( d->cursor_save.X, d->cursor_save.Y, d );
				}
				
				//std::cout << "Mouse UP " << SDL_GetTicks() << std::endl;
				d->Translating = false;
				d->Rotating = false;
				d->Zooming = false;
				// !!!! ****  Show Cursor  **** !!!!
				
				SDL_ShowCursor( 1 );
			}
		} 
		
		unlock( d );
		
	} // end doMovementMaya
	
	
	
	
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
	
	void Camera::lock( Device * d )
	{
		SDL_LockMutex( d->cam_mutex );
	}
	
	void Camera::unlock( Device * d )
	{
		SDL_UnlockMutex( d->cam_mutex );
	}


}

