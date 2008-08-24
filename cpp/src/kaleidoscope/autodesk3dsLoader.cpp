/*
 *  autodesk3dsLoader.cpp
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 8/22/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */

#include <kaleidoscope/autodesk3dsLoader.h>


namespace Kaleidoscope
{
	
	Autodesk3dsLoader::Autodesk3dsLoader()
	{
	
	}
	
	void Autodesk3dsLoader::load3ds( Device * d, GridsID new_id, std::string file_name, bool show_lines )
	{
		float new_color[] = { 1.0f, 1.0f, 1.0f, 0.6f };
		
		load3ds( d, new_id, file_name, &new_color[0], show_lines );
	}
	
	
	void Autodesk3dsLoader::load3ds( Device * d, GridsID new_id, std::string file_name, float * color, bool show_lines )
	{
		Scene3DS * scene = new Scene3DS( file_name );
	
		std::vector<Mesh3DSObject> meshes = scene->Meshes();
		
		
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "r" ] = color[ 0 ];
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "g" ] = color[ 1 ];
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "b" ] = color[ 2 ];
		d->world_hash[ new_id ][ "Color" ][ 0u ][ "a" ] = color[ 3 ];
		
		if( show_lines )
		{
			d->world_hash[ new_id ][ "Color" ][ 1u ][ "r" ] = 0.0f;
			d->world_hash[ new_id ][ "Color" ][ 1u ][ "g" ] = 0.0f;
			d->world_hash[ new_id ][ "Color" ][ 1u ][ "b" ] = 0.0f;
			d->world_hash[ new_id ][ "Color" ][ 1u ][ "a" ] = 0.75f;
		}
		
		int vertex_counter = 0;
		
		if(  meshes.size() > 0 )
		{
			for( int i = 0; i < meshes.size(); i++ )
			{
				std::vector<Mesh3DSVertex> loaded_vertices = meshes[i].Vertices();
				
				int g;
				for( g = 0; g < loaded_vertices.size(); g++ )
				{
					d->world_hash[ new_id ][ "Vertices" ][ g + vertex_counter][ "x" ] = loaded_vertices[ g ].x;
					d->world_hash[ new_id ][ "Vertices" ][ g + vertex_counter][ "y" ] = loaded_vertices[ g ].y;
					d->world_hash[ new_id ][ "Vertices" ][ g + vertex_counter][ "z" ] = loaded_vertices[ g ].z;
				}
				
				std::vector<Mesh3DSFace> loaded_faces = meshes[i].Faces();
				
				std::cout << "Loading " << loaded_faces.size() << " polygons.\n";
				
				d->world_hash[ new_id ][ "Triangles" ][ i ][ "Color" ] = 0u;
				
				for( int h = 0; h < loaded_faces.size(); h++ )
				{
					d->world_hash[ new_id ][ "Triangles" ][ i ][ "Indices" ][ h ][ 0u ] = loaded_faces[ h ].a + vertex_counter;
					d->world_hash[ new_id ][ "Triangles" ][ i ][ "Indices" ][ h ][ 1u ] = loaded_faces[ h ].b + vertex_counter;
					d->world_hash[ new_id ][ "Triangles" ][ i ][ "Indices" ][ h ][ 2u ] = loaded_faces[ h ].c + vertex_counter;
				}
				
				
				if( show_lines )
				{
					d->world_hash[ new_id ][ "Lines" ][ i ][ "Color" ] = 1u;
					
					int j = 0;
				
					for( int h = 0; h < loaded_faces.size(); h++ )
					{
						d->world_hash[ new_id ][ "Lines" ][ i ][ "Indices" ][ j ][ 0u ] = loaded_faces[ h ].a + vertex_counter;
						d->world_hash[ new_id ][ "Lines" ][ i ][ "Indices" ][ j ][ 1u ] = loaded_faces[ h ].b + vertex_counter;
						
						d->world_hash[ new_id ][ "Lines" ][ i ][ "Indices" ][ j+1 ][ 0u ] = loaded_faces[ h ].a + vertex_counter;
						d->world_hash[ new_id ][ "Lines" ][ i ][ "Indices" ][ j+1 ][ 1u ] = loaded_faces[ h ].c + vertex_counter;
						
						d->world_hash[ new_id ][ "Lines" ][ i ][ "Indices" ][ j+2 ][ 0u ] = loaded_faces[ h ].b + vertex_counter;
						d->world_hash[ new_id ][ "Lines" ][ i ][ "Indices" ][ j+2 ][ 1u ] = loaded_faces[ h ].c + vertex_counter;
						
						j += 3;
					}
				
				} // end if show_lines
				
				
				vertex_counter += g;
				
			} // end for each mesh in file
		} // end if size > 0
		else
		{
			std::cout << "Error Loading model" << std::endl;
		}
	
	} // end load3ds


} // end namespace Kaleidoscope

