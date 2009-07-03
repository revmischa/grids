/*
 *  device.h
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
#include <kaleidoscope/renderer.h>
#include <kaleidoscope/eventController.h>
#include <kaleidoscope/camera.h>
#include <kaleidoscope/cursorController.h>
#include <kaleidoscope/room.h>
#include <kaleidoscope/builder.h>
#include <kaleidoscope/guiItem.h>
#include <kaleidoscope/gui.h>
#include <kaleidoscope/RenderObject.h>
#include <kaleidoscope/autodesk3dsLoader.h>
#include <kaleidoscope/voxelSpace.h>
#include <kaleidoscope/threadController.h>

#include <grids/interface.h>
#include <grids/value.h>
#include <grids/utility.h>
#include <grids/otr.h>
#include <grids/uuid.h>

#include <SDL/SDL.h>

#include <vector>
#include <map>

#include <SDL_ttf/SDL_ttf.h>

extern "C" {

     #include <libotr/proto.h>
     #include <libotr/message.h>
     #include <libotr/privkey.h>
}


namespace Grids
{
	class Interface;
	class Utility;
	class OTR;
}



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
	class VoxelSpace;
	class ThreadController;
	class Autodesk3dsLoader;
	class GuiItem;


	class Device
	{
	public:

		Device( int, int );
		~Device();
		
        // return 0 if succeed, -1 if fail 
		int initSDL();
		
		void run();
		
		Gui * createGui();
		Grids::Interface * createInterface( std::string );
		VoxelSpace * createVoxel();

		Renderer * getRenderer();
		EventController * getEventController();
		Camera * getCamera();
		CursorController * getCursorController();
		Builder * getBuilder();
		Gui * getGui();
		Autodesk3dsLoader * getLoader();
		VoxelSpace * getVoxel();
		ThreadController * getThreadController();
	
		Grids::Interface * getInterface( );
		
		void setRenderer( Renderer * );
		void setEventController( EventController * );
		void setCamera( Camera * );
		void setCursorController( CursorController * );
		void setBuilder( Builder * );
		void setGui( Gui * );
		void setLoader( Autodesk3dsLoader * );
		void setVoxel( VoxelSpace * );
		void setThreadController( ThreadController * );
		void setInterface( Grids::Interface * );
		
		void lockDevice();
		void unlockDevice();
		
		void lockWorldHash();
		void unlockWorldHash();

		Grids::GridsID getMyID();
				
		void setRoomWidth( float );
		float getRoomWidth();

		void addRoom( Room * );

		void setMyRoom( Grids::GridsID );
		Grids::GridsID getMyRoom();
		std::vector< Grids::GridsID > getKnownRooms();

		////////////////////////////////////
		//// OTR Methods
		///////////////////////////////////

		// Nota Bene : typedef struct s_OtrlUserState* OtrlUserState;
		// OtrlUserStates are already pointers

		OtrlUserState getOtrUserState();
		void setOtrUserState( OtrlUserState );

		OtrlMessageAppOps*  getOtrOps();

		std::string getKeyFile();
		void setKeyFile( std::string );
		std::string getFingerprintFile();
		void setFingerprintFile( std::string );

		std::string getProtocol();

		
		//////////////////////////////////
		// MEMEBERS
		/////////////////////////////////

		// Debug levels
		// 1 - 
		// 2 - 
		// 3 - 
		// 4 - Key presses
		
		int DEBUG;

		// ******  MAKE THIS A POINTER

		Grids::Value  world_hash;


		int running;

		int last_clock;

		float room_width;
	
		SDL_Event device_event;
		
		std::string temp_room_id;
		
		std::string newUUID();

		///////////////////////////////
		// Camera members
		/////////////////////////////

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

		float Fov;
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
		// Render members
		////////////////


		int width;
		int height;
		int x_pos; // stores the window's upper left x position
		int y_pos; // stores the window's upper right y position

		int current_text_mode;
		char * text_mode_string[4]; 
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
		// Event members
		/////////////

		int mouse_x;
		int mouse_y;
	
		int down_x;
		int down_y;
	
		bool mouse_moved;
	
		bool mouse_down;
	
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
		//  Cursor members
		///////////////

		int last_x;
		int last_y;
	
		///////////////
		//  Gui members
		///////////////
		
		Grids::Value text_hash;
		std::vector< SDL_Surface * > screen_texts;
		std::vector< SDL_Surface * > space_texts;
	
		SDL_Surface * loaded_image;
				
		//int screen_font;
		//int space_font;
		TTF_Font * screen_font; // Deleted ~Device()->deleteSDLFonts()
		TTF_Font * space_font;  // Deleted ~Device()->deleteSDLFonts()
		SDL_Color text_color;
	
		GLuint texture;
	
		std::map< Grids::GridsID, SDL_mutex* > text_box_suface_mutexes;
		std::map< Grids::GridsID, SDL_Surface* > text_box_surfaces;
	
	
		///////////////
		//  Multithreading Variables
		///////////////
		
		// All deleted ~Device()
		
		SDL_mutex * device_mutex; 
		
		SDL_mutex * cursor_controller_mutex;
		SDL_mutex * renderer_mutex;
		SDL_mutex * event_controller_mutex;
		SDL_mutex * cam_mutex;
		
		SDL_mutex * builder_mutex;
		SDL_mutex * loader_mutex;
		SDL_mutex * thread_controller_mutex;
		SDL_mutex * voxel_mutex;
		
		SDL_mutex * gui_mutex; // Deleted in ~ObjectController()
		SDL_mutex * interface_mutex;
		
		SDL_mutex * world_hash_mutex;
		
		
		SDL_Thread * voxel_update_thread; // Deleted ?
		SDL_Thread * create_room_debug_thread;
		SDL_Thread * selection_thread;
		
		
		Grids::GridsID temp_box_id;

	private:

		Grids::UUID device_uuid_obj;

		void loadIDFromFile();
		
		SDL_Surface *gScreen;
		
		Grids::GridsID my_room;
		Grids::GridsID my_id;

		std::string keyfile;
		std::string fprfile;

		// All deleted in ~Device()
		Renderer * renderer;
		EventController * event_controller;
		Camera * cam;
		CursorController * cursor_controller;

		Builder * builder;
		Gui * gui;
		Autodesk3dsLoader * loader;
		VoxelSpace * voxel;
		
		ThreadController * thread_controller;
		Grids::Interface * interface;
		
		void createMutexes();

		void deleteSDLSurfaces();
		void deleteSDLTextSurfaces();
		void deleteSDLFonts();

		OtrlUserState otr_user_state;

		void initOtr();

		
		
	};

} // end namespace Kaleidoscope
