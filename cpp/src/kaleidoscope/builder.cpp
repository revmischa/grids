/*
 *  builder.cpp
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 8/20/08.
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

#include <kaleidoscope/builder.h>
#include <kaleidoscope/simpleCube.h>

#include <grids/define.h>

#include <math.h>
#include <SDL_image/SDL_image.h>



namespace Kaleidoscope
{

	Builder::Builder()
	{

	}

	void Builder::placeObject( Device * d, GridsID new_id, GridsID Room_ID, Vec3D new_position, Vec3D new_scale, Vec3D new_rotation )
	{
		d->lockWorldHash();

		if( !( d->world_hash[ Room_ID ][ "objects" ] ) )
		{
			d->world_hash[ Room_ID ][ "objects" ][ 0u ] = new_id;
		}
		else
		{
			int temp_index = d->world_hash[ Room_ID ][ "objects" ].size();
			
			bool id_in_room = false;
			
			for( int i = 0; i < temp_index; i++ )
			{
				if( new_id == d->world_hash[ Room_ID ][ "objects" ][ i ].asString() )
				{
					id_in_room = true;
				}
			}
			
			if( !id_in_room ){
				d->world_hash[ Room_ID ][ "objects" ][ temp_index ] = new_id;
			}
		}

		d->world_hash[ new_id ][ "room" ] = Room_ID;

		d->world_hash[ new_id ][ "position" ][ 0u ] = new_position.X;
		d->world_hash[ new_id ][ "position" ][ 1u ] = new_position.Y;
		d->world_hash[ new_id ][ "position" ][ 2u ] = new_position.Z;

		d->world_hash[ new_id ][ "scale" ][ 0u ] = new_scale.X;
		d->world_hash[ new_id ][ "scale" ][ 1u ] = new_scale.Y;
		d->world_hash[ new_id ][ "scale" ][ 2u ] = new_scale.Z;

		d->world_hash[ new_id ][ "rotation" ][ 0u ] = new_rotation.X;
		d->world_hash[ new_id ][ "rotation" ][ 1u ] = new_rotation.Y;
		d->world_hash[ new_id ][ "rotation" ][ 2u ] = new_rotation.Z;
		
		d->unlockWorldHash();

	}



	void Builder::placeObject( Device * d, GridsID new_id, Vec3D new_position, Vec3D new_scale, Vec3D new_rotation  )
	// Place an object that's not contained inside a room
	{
		

	}

	void Builder::buildChair( Device * d, GridsID new_id )
	{
		d->lockWorldHash();
		
		d->world_hash[ new_id ][ "quads" ] = Grids::Value();

		d->world_hash[ new_id ][ "color" ] = Grids::Value();
		d->world_hash[ new_id ][ "color" ][ 0u ] = Grids::Value();

		d->world_hash[ new_id ][ "color" ][ 0u ][ 0u ] = 0.9f;
		d->world_hash[ new_id ][ "color" ][ 0u ][ 1u ] = 0.2f;
		d->world_hash[ new_id ][ "color" ][ 0u ][ 2u ] = 0.2f;
		d->world_hash[ new_id ][ "color" ][ 0u ][ 3u ] = 0.75f;

		// Quads have 4 vertices, so 4 more hashes are needed

		d->world_hash[ new_id ][ "vertices" ][ 0u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 0u ][ 0u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 0u ][ 1u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 0u ][ 2u ] = -1.0f;

		d->world_hash[ new_id ][ "vertices" ][ 1u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 1u ][ 0u ] = 1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 1u ][ 1u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 1u ][ 2u ] = -1.0f;

		d->world_hash[ new_id ][ "vertices" ][ 2u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 2u ][ 0u ] = 1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 2u ][ 1u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 2u ][ 2u ] = 1.0f;

		d->world_hash[ new_id ][ "vertices" ][ 3u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 3u ][ 0u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 3u ][ 1u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 3u ][ 2u ] = 1.0f;

		// Quads have 4 vertices, so 4 more hashes are needed

		d->world_hash[ new_id ][ "vertices" ][ 4u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 4u ][ 0u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 4u ][ 1u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 4u ][ 2u ] = -1.3f;

		d->world_hash[ new_id ][ "vertices" ][ 5u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 5u ][ 0u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 5u ][ 1u ] = 1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 5u ][ 2u ] = -1.3f;

		d->world_hash[ new_id ][ "vertices" ][ 6u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 6u ][ 0u ] = 1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 6u ][ 1u ] = 1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 6u ][ 2u ] = -1.3f;

		d->world_hash[ new_id ][ "vertices" ][ 7u ] = Grids::Value();
		d->world_hash[ new_id ][ "vertices" ][ 7u ][ 0u ] = 1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 7u ][ 1u ] = -1.0f;
		d->world_hash[ new_id ][ "vertices" ][ 7u ][ 2u ] = -1.3f;

		//d->world_hash[ new_id ][ "lines" ] = Grids::Value();

		d->world_hash[ new_id ][ "quads" ][ 0u ] = Grids::Value();
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "color" ] = 0u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ] = Grids::Value();
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ] = Grids::Value();
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ] = Grids::Value();


		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 0u ] = 0u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 1u ] = 1u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 2u ] = 2u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 0u ][ 3u ] = 3u;

		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 0u ] = 4u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 1u ] = 5u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 2u ] = 6u;
		d->world_hash[ new_id ][ "quads" ][ 0u ][ "indices" ][ 1u ][ 3u ] = 7u;
		
		d->unlockWorldHash();
	}
	
	void Builder::buildBillboard( Device * d, GridsID new_id, std::string image_name )
	// Builds and attaches a glow orb to the given GridsID
	{
		SDL_Surface * temp_surface = loadImage( image_name );

		// pack image pixles into json... hmmm
	}
	
	SDL_Surface * Builder::loadImage( std::string image_name )
	{
		return IMG_Load( image_name.c_str() );
	}

	
	void Builder::createRandomBoxes( Device * d, Grids::GridsID room_id, int num_boxes )
	{
		
		float temp_box_color[ 4 ];
		
		float room_width = d->getRoomWidth();
		
		for( int i = 0; i < num_boxes; i++ )
		{
			temp_box_color[ 0 ] = (rand() % 10000)/10000.0f;
			temp_box_color[ 1 ] = (rand() % 10000)/10000.0f;
			temp_box_color[ 2 ] = (rand() % 10000)/10000.0f;
			temp_box_color[ 3 ] = 0.35f;
			
			if( d->DEBUG > 2 ){
				std::cout << "Builder, requesting cube" << std::endl;
			}

			SimpleCube::requestCreate( d, room_id, Vec3D( room_width - (rand() % 10000)/10000.0f * room_width * 2.0f,  
																									 room_width - (rand() % 10000)/10000.0f * room_width * 2.0f , 
																									 room_width - (rand() % 10000)/10000.0f * room_width * 2.0f ),
										  2.0f, &temp_box_color[ 0 ]  );
		}
		
		
	}
	
	
	int Builder::packImage( Device * d, GridsID in_id, SDL_Surface * in_surface )
	{
		if( !( d->world_hash[ in_id ][ "images" ] ) ) // if there are no images attached to the array
		{
			d->world_hash[ in_id ][ "images" ][ 0u ] = Grids::Value();			
			
			int surface_x = in_surface->w;
			int surface_y = in_surface->h;
			
			for( int i = 0; i < surface_x; i++ )
			{
				for( int g = 0; g < surface_y; g++ )
				{
					d->world_hash[ in_id ][ "images" ][ 0u ][ i ][ g ] = getPixel(in_surface, i, g);
				}
			}
			
			return 0;
			
		} // end if "images" is empty
		else
		{
			int new_index = d->world_hash[ in_id ][ "images" ].size();
			
			int surface_x = in_surface->w;
			int surface_y = in_surface->h;
			
			for( int i = 0; i < surface_x; i++ )
			{
				for( int g = 0; g < surface_y; g++ )
				{
					d->world_hash[ in_id ][ "images" ][ new_index ][ i ][ g ] = getPixel(in_surface, i, g);
				}
			}
			
			return new_index;
		}
		
		
	} // end packImage()
	
	
	SDL_Surface * Builder::getImage( Device * d, GridsID in_id )
	{
		return getImage( d, in_id, 0u );
	}
	
	SDL_Surface * Builder::getImage( Device * d, GridsID in_id, int in_index )
	{
		int surface_x = d->world_hash[ in_id ][ "images" ][ in_index ].size();
		int surface_y = d->world_hash[ in_id ][ "images" ][ in_index ][ 0u ].size(); // assuming of course that this as rectangular...
		
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				Uint32 rmask = 0xff000000;
				Uint32 gmask = 0x00ff0000;
				Uint32 bmask = 0x0000ff00;
				Uint32 amask = 0x000000ff;
		#else
				Uint32 rmask = 0x000000ff;
				Uint32 gmask = 0x0000ff00;
				Uint32 bmask = 0x00ff0000;
				Uint32 amask = 0xff000000;
		#endif
				
		SDL_Surface * temp_surface = SDL_CreateRGBSurface( SDL_HWSURFACE, surface_x, surface_y, 32, rmask, gmask, bmask, amask );
		
		SDL_LockSurface( temp_surface );
		
		// modify pixels
		
		Uint32 temp_pixel;
		
		for( int i = 0; i < surface_x; i++ )
		{
			for( int g = 0; g < surface_y; g++ )
			{
				temp_pixel = d->world_hash[ in_id ][ "images" ][ in_index ][ i ][ g ].asDouble();
				
				putPixel( temp_surface, i, g, temp_pixel );
			}
		}
		
		SDL_UnlockSurface( temp_surface );
		
		return temp_surface;
		
	}
	
	
	
	
	
	
	/////////////////////
	///  PRIVATE
	////////////////////
	
	Uint32 Builder::getPixel(SDL_Surface *surface, int x, int y)
	{
		int bpp = surface->format->BytesPerPixel;
		/* Here p is the address to the pixel we want to retrieve */
		Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
		
		switch(bpp) {
			case 1:
				return *p;
				
			case 2:
				return *(Uint16 *)p;
				
			case 3:
				if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
					return p[0] << 16 | p[1] << 8 | p[2];
				else
					return p[0] | p[1] << 8 | p[2] << 16;
				
			case 4:
				return *(Uint32 *)p;
				
			default:
				return 0;       /* shouldn't happen, but avoids warnings */
		}
	}
	
	void Builder::putPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
	{
		int bpp = surface->format->BytesPerPixel;
		/* Here p is the address to the pixel we want to set */
		Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
		
		switch(bpp) {
			case 1:
				*p = pixel;
				break;
				
			case 2:
				*(Uint16 *)p = pixel;
				break;
				
			case 3:
				if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
					p[0] = (pixel >> 16) & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = pixel & 0xff;
				} else {
					p[0] = pixel & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = (pixel >> 16) & 0xff;
				}
				break;
				
			case 4:
				*(Uint32 *)p = pixel;
				break;
		}
	}
	
	void Builder::lock( Device * d )
	{
		SDL_LockMutex( d->builder_mutex );
	}
	
	void Builder::unlock( Device * d )
	{
		SDL_UnlockMutex( d->builder_mutex );
	}

} // end namespace Kaleidoscope

