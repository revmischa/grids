/*
 *  GridsObject.h
 *  grids_view_01
 *
 *  Created by Patrick Tierney on 8/7/08.
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
#include <grids/define.h>


#include <vector>
#include <SDL/SDL.h>

namespace Kaleidoscope
{
	class Builder;
	class Device;	
}

namespace Grids
{
	class ObjectController;

	class Object
	{
	public:

		Object( Kal::Device*, Value* );
		~Object();

		static GridsID requestCreate( Kal::Device* d, GridsID, Value* );
		virtual void create( Kaleidoscope::Device *, Value* ) = 0;
			
		void requestUpdatePosition( Kaleidoscope::Device *, Vec3D, Vec3D, Vec3D );
		void updatePosition( Kaleidoscope::Device *, Value * );

	
		static std::string getNameFromValue( Value* );
		static void loadPosition( Value* temp_value, Vec3D pos, Vec3D rot, Vec3D scl );
		static void setRoom( Value*, GridsID );

		virtual GridsID getID( Kal::Device* );
		virtual GridsID getID();
		virtual void setID( GridsID );
	
		virtual GridsID getRoomID();
		virtual void setRoomID( GridsID );

		virtual void prepDraw();
		virtual void draw( Kal::Device* ) = 0; // pure virtual function
		virtual void finDraw(); 
	
		virtual void drawAll( Kal::Device* );


		virtual Vec3D getPosition(  );
		virtual Vec3D getAttrPosition(  );
			
		virtual Vec3D getScale( );
		virtual Vec3D getAttrScale( );

		virtual Vec3D getRotation( );
		virtual Vec3D getAttrRotation( );
		
		void setAttrPosition( Vec3D );
		void setAttrRotation( Vec3D );
		void setAttrScale( Vec3D );
	
		void storePositionFromAttr( Value* in_val );
	
		void setPosition( Kal::Device*, Vec3D );

		void setController( ObjectController * );
	
		float detectSelection( Kaleidoscope::Device *, GridsID, Vec3D, Vec3D );
		
		void selectObject( Kaleidoscope::Device * );
		
		/* This should be overloaded for more complex objects (models, etc) */
		float calculateRadius( Kaleidoscope::Device *, GridsID );
	
		/* calculateRadiusFromVertices adds a [ this_id ][ "radius" ] entry to the world value LOCALLY */
		float calculateRadiusFromVertices( Kaleidoscope::Device *, GridsID );
		
		virtual std::vector< Object* > getChildren();
		virtual void addChild( Object* );
		virtual void deleteChild( Object* );
		void deleteChildren( );		

		void setParent( Object* );
		static void setParentValue(  Value*, GridsID );
		Object* getParent();

		Object* getParentFromAttr( Kal::Device*, Value* ); 
		GridsID getParentIDFromAttr( Value* );

		Object* getParentFromValue( Kal::Device*, Value* ); 
		GridsID getParentIDFromValue( Value* );


		/* Locking / unlocking should always be done through the base class. */
		void initMutex();
		void deleteMutex();
		void lock();
		void unlock();

		Value* getAttr();		
		static Value* getAttr( Value* in_val );

		bool getVisibility();
		void hide();
		void show();		
 
	protected:
		
		virtual GridsID getIDFromValue( Value* );

		bool is_visible;

		Value attr;		
		
		GridsID this_id;
		GridsID room_id;
		
		ObjectController * controller;
		
		Object* parent;

		
		
	private:
		friend class Kal::Builder;
		
		SDL_mutex * object_mutex;
			
		std::vector< Object* > children;
	};

} // end namespace Grids

