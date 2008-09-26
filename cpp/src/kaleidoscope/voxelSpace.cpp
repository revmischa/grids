/*
 *  voxelSpace.cpp
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 9/11/08.
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

#include <kaleidoscope/voxelSpace.h>


#define NODE_POTENTIAL 225.0f

namespace Kaleidoscope 
{
		
	VoxelSpace::VoxelSpace( Grids::GridsID in_id, float ww, float hh, float dd, float xs, float ys, float zs ) 
	// ww, hh, dd = the width, height, and depth of the space to analize
	// xs, ys, zs = the number of steps along each axis
	
	{
		voxel_id = in_id;
		
		std::vector< int > temp_vector( 2 );
		std::vector< int > temp_vector_2( 5 );
		
		for( int i = 0; i < 4; i++ )
		{
			_edges.push_back( temp_vector );
		}
		
		for( int i = 0; i < 4; i++ )
		{
			_offsets.push_back( temp_vector );
		}
		
		for( int i = 0; i < 16; i++ )
		{
			_lines.push_back( temp_vector_2 );
		}
		
		_edges[ 0u ][ 0u ] = 0;  _edges[ 0u ][ 1u ] = 1; 
		_edges[ 1u ][ 0u ] = 1;  _edges[ 1u ][ 1u ] = 2;
		_edges[ 2u ][ 0u ] = 2;  _edges[ 2u ][ 1u ] = 3;
		_edges[ 3u ][ 0u ] = 3;  _edges[ 3u ][ 1u ] = 0;
		
		_offsets[ 0u ][ 0u ] = 0; _offsets[ 0u ][ 1u ] = 0;
		_offsets[ 1u ][ 0u ] = 1; _offsets[ 1u ][ 1u ] = 0;
		_offsets[ 2u ][ 0u ] = 1; _offsets[ 2u ][ 1u ] = 1;
		_offsets[ 3u ][ 0u ] = 0; _offsets[ 3u ][ 1u ] = 1;
		
		_lines[0][0] = -1; _lines[0][1] = -1; _lines[0][2] = -1; _lines[0][3] = -1; _lines[0][4] = -1;
		_lines[1][0] = 0; _lines[1][1] = 3; _lines[1][2] = -1; _lines[1][3] = -1; _lines[1][4] = -1;
		_lines[2][0] = 0; _lines[2][1] = 1; _lines[2][2] = -1; _lines[2][3] = -1; _lines[2][4] = -1;
		_lines[3][0] = 3; _lines[3][1] = 1; _lines[3][2] = -1; _lines[3][3] = -1; _lines[3][4] = -1;
		_lines[4][0] = 1; _lines[4][1] = 2; _lines[4][2] = -1; _lines[4][3] = -1; _lines[4][4] = -1;
		_lines[5][0] = 1; _lines[5][1] = 2; _lines[5][2] = 0; _lines[5][3] = 3; _lines[5][4] = -1;
		_lines[6][0] = 0; _lines[6][1] = 2; _lines[6][2] = -1; _lines[6][3] = -1; _lines[6][4] = -1;
		_lines[7][0] = 3; _lines[7][1] = 2; _lines[7][2] = -1; _lines[7][3] = -1; _lines[7][4] = -1;
		_lines[8][0] = 3; _lines[8][1] = 2; _lines[8][2] = -1; _lines[8][3] = -1; _lines[8][4] = -1;
		_lines[9][0] = 0; _lines[9][1] = 2; _lines[9][2] = -1; _lines[9][3] = -1; _lines[9][4] = -1;
		_lines[10][0] = 3; _lines[10][1] = 2; _lines[10][2] = 0; _lines[10][3] = 2; _lines[10][4] = -1;
		_lines[11][0] = 1; _lines[11][1] = 2; _lines[11][2] = -1; _lines[11][3] = -1; _lines[11][4] = -1;
		_lines[12][0] = 3; _lines[12][1] = 1; _lines[12][2] = -1; _lines[12][3] = -1; _lines[12][4] = -1;
		_lines[13][0] = 0; _lines[13][1] = 1; _lines[13][2] = -1; _lines[13][3] = -1; _lines[13][4] = -1;
		_lines[14][0] = 0; _lines[14][1] = 3; _lines[14][2] = -1; _lines[14][3] = -1; _lines[14][4] = -1;
		_lines[15][0] = -1; _lines[15][1] = -1; _lines[15][2] = -1; _lines[15][3] = -1; _lines[15][4] = -1;
	
		w = ww;
		h = hh;
		depth = dd;
		
		w2 = w / 2.0f;
		d2 = depth / 2.0f;
		h2 = h / 2.0f;
		
		n_w2 = -w2;
		n_h2 = -h2;
		n_d2 = -d2;
		
		xscale = (float)w/xs;
		yscale = (float)h/ys;
		zscale = (float)depth/zs;
		gridX = (xs+1);
		gridY = (ys+1);
		gridZ = (zs+1);
		
		std::vector< float > temp_vector_3( gridY );
		for( int i = 0; i < gridX; i++ )
		{
			_sliceData.push_back( temp_vector_3 );
		}
		
		std::vector< bool > temp_vector_4( gridY );
		for( int i = 0; i < gridX; i++ )
		{
			_edgeFlags.push_back( temp_vector_4 );
		}
	
	}
	
	// updates node list, called from showNodes() above
	void VoxelSpace::setNodes( Device * d ) 
	{		
		node_positions.clear();
		
		int num_objects;
		Grids::GridsID temp_room;
		bool objects_dont_exist;
		
		d->lockWorldHash();
		// set the vector node_positions from the relevant people and objects
		int num_rooms = d->world_hash[ "rooms" ].size();
		d->unlockWorldHash();
			
		
		for( int i = 0; i < num_rooms; i++ )
		{
			d->lockWorldHash();
			temp_room = d->world_hash[ "rooms" ][ i ].asString();
			objects_dont_exist = !(d->world_hash[ temp_room ][ "objects" ]);
			
			if( !objects_dont_exist )
			{
				num_objects = d->world_hash[ temp_room ][ "objects" ].size();
			}
			
			d->unlockWorldHash();
			
			
			if(  objects_dont_exist == false )
			{
				for( int g = 0; g < num_objects; g++ )
				{
					d->lockWorldHash();
				
					GridsID temp_object = d->world_hash[ temp_room ][ "objects" ][ g ].asString();
					
					node_positions.push_back( Vec3D( d->world_hash[ temp_object ][ "position" ][ 0u ].asDouble() + 
													d->world_hash[ temp_room ][ "position" ][ 0u ].asDouble(),
													
													d->world_hash[ temp_object ][ "position" ][ 1u ].asDouble() + 
													d->world_hash[ temp_room ][ "position" ][ 1u ].asDouble(),
													
													d->world_hash[ temp_object ][ "position" ][ 2u ].asDouble()	+ 
													d->world_hash[ temp_room ][ "position" ][ 2u ].asDouble()  )  );
					d->unlockWorldHash();
					
				}
			}
		}
		
		numNodes = node_positions.size();
		
		createPotentials();
	}
	
	void VoxelSpace::createPotentials( )
	// You can think of the potetnials as the radius of the sphere that each Node will create
	{
		node_potentials.clear();
		
		for( int i = 0; i < node_positions.size(); i++ )
		{
			node_potentials.push_back( NODE_POTENTIAL );
		}
	}                            
		
	
	// compute new surface hull
	// s=step size
	// isoV = threshold value to be used for finding surface
	void VoxelSpace::update( Device * d, float s, float isoV) 
	{
		setNodes( d );
		
		vertex_counter = 0;
		line_counter = 0;
		
		d->lockWorldHash();
		//I think that this block of code the updates the world hash is not necessory
		
		// If I clear these JSon values
		//d->world_hash[ voxel_id ][ "vertices" ].clear(); // clear the vertices
		//d->world_hash[ voxel_id ][ "lines" ].clear(); // stores the color and indices
		//d->world_hash[ voxel_id ][ "color" ].clear();
		
		d->world_hash[ voxel_id ][ "color" ][ 0u ][ 0u ] = 1.0f;
		d->world_hash[ voxel_id ][ "color" ][ 0u ][ 1u ] = 1.0f;
		d->world_hash[ voxel_id ][ "color" ][ 0u ][ 2u ] = 1.0f;
		d->world_hash[ voxel_id ][ "color" ][ 0u ][ 3u ] = 0.20f;
		
		d->world_hash[ voxel_id ][ "lines" ][ 0u ][ "color" ] = 0u;
		
		d->world_hash[ voxel_id ][ "position" ][ 0u ] = w / 2.0f;
		d->world_hash[ voxel_id ][ "position" ][ 1u ] = 0.0f;
		d->world_hash[ voxel_id ][ "position" ][ 2u ] = depth / 2.0f;
		
		d->unlockWorldHash();
				
		vCount=0;
		for(float i=-depth/2.0f; i<= depth/2.0f; i+=s)
		{
			generateVoxelSlice(d, true, i, isoV);
			generateVoxelSlice(d, false, i, isoV);
		}
		
		//node_positions.clear();
		//node_potentials.clear();
	}
	
	// Rendering is done in the Render class
	void VoxelSpace::render() 
	{

	}
	
	// marching square implementation based on code/descriptions by paul bourke
	void VoxelSpace::generateVoxelSlice( Device * d, bool zSlice,float currSlice, float isoV) 
	{		
		float sum, dx, dy, dz, distsq, currX, currY, currZ;
		// decide if XY or ZY plane
		// only difference between the 2 cases is that
		// we either iterate along the X or Z axis
		// the algorithm starts computing the strength of all nodes at all
		// grid points on the selected slice/plane
		// each result is stored in the 2d _sliceData array for the next part
		// the result values can also be understood as field strength 
		if (zSlice) 
		{
			currX = n_w2;
			for (int x = 0; x < gridX; x++) 
			{
				currY = n_h2;
				for (int y = 0; y < gridY; y++) 
				{
					// reset all edges to unused
					_edgeFlags[x][y] = false;
					sum = 0;
					// iterate through all nodes and sum up their potential
					// based on their distance to the current grid point
					// the further away a node, the less impact it has on that point
					for(int i=0; i<numNodes; i++)
					{
						dx = currX - node_positions[i].X;
						dy = currY - node_positions[i].Y;
						dz = currSlice - node_positions[i].Z;
						// simplified pythagorean, only uses squared distance to avoid sqrt()
						// 0.0001 offset to avoid division by 0
						sum += node_potentials[i] / ( (dx*dx+dy*dy+dz*dz)+0.0001 );
					} // end i = 0
					_sliceData[x][y] = sum;
					// step along Y axis
					currY += yscale;
				} // end for y = 0
				// step along X
				currX += xscale;
			} // end for x = 0
			currSlice-=d2;
			
		} // end if zSlice 
		else 
		{
			currZ = n_d2;
			// same steps as above, only in ZY plane
			for (int z = 0; z < gridZ; z++) 
			{
				currY = n_h2;
				for (int y = 0; y < gridY; y++) 
				{
					_edgeFlags[z][y] = false;
					sum = 0;
					for(int i=0; i<numNodes; i++)
					{
						dx = currSlice - node_positions[i].X;
						dy = currY - node_positions[i].Y;
						dz = currZ - node_positions[i].Z;
						sum += node_potentials[i]/((dx*dx+dy*dy+dz*dz)+0.0001);
					}
					_sliceData[z][y] = sum;
					currY += yscale;
				}
				currZ += zscale;
			}
			currSlice-=w2;
		}
		
		// those values are described below
		std::vector< int > startP; // array
		std::vector< int > endP; // array
		int startOffsetX;
		int startOffsetY;
		int endOffsetX;
		int endOffsetY;
		int corners;
		std::vector< int > currEdges; // array
		int yy,n;
		float v1;
		float dV;
		float lerpF;
		float vx1,vy1,vx2,vy2;
		
		// main slice rendering pass
		// re-process values in _sliceData array and determine if
		// the field strength at each point is less than the given
		// threshold value. we have found an intersection with the 
		// boundary surface, if the current point's value is below,
		// but one of it's neighbour is >= threshold value
		
		int gX1=(zSlice ? gridX-1 : gridZ-1);
		int gY1=gridY-1;
		
		for (int y = 0; y < gY1; y++) 
		{
			yy=y+1;
			for (int x = 0,xx=1; x < gX1; x++) 
			{
				// check if this edge hasn't already been processed
				if ( !_edgeFlags[x][y] ) 
				{
					corners = 0;
					// compare all 4 corners of the current grid square/cell with
					// the iso threshold value and set respective codes
					// 1=bottom left
					// 2=bottom right
					// 4=top right
					// 8=top left
					if (_sliceData[x][y] < isoV) 
					{
						corners |= 1;
					}
					if (_sliceData[xx][y] < isoV) 
					{
						corners |= 2;
					}
					if (_sliceData[xx][yy] < isoV) 
					{
						corners |= 4;
					}
					if (_sliceData[x][yy] < isoV) 
					{
						corners |= 8;
					}
					
					// if the result of corners=0 the entire square is within the surface area
					// if it is 1+2+4+8=15 the square is totally outside
					// in both cases no further steps are needed and we can skip that next part
										
					if (corners > 0 && corners < 0x0f)  
					{
						
						n = 0;
						// here the algorithm makes heavy use of the lookup tables (defined above)
						// to compute the exact position of the surface line(s) in the current cell and at same
						// time take care of all possible symmetries.
						// note that there're 2 special cases where there will be 2 lines crossing the cell
						// this only happens when diagonally opposite points are within the surface and the other 2 outside
						currEdges = _lines[corners];
						// a value of -1 in the currEdges array means no more edges to check
						while (currEdges[n] != -1) 
						{
							// retrieve edges involved
							std::vector< int > edge1 = _edges[currEdges[n++]];
							std::vector< int > edge2 = _edges[currEdges[n++]];
							
							// get indexes of start/end point relative to current grid xy position
							startP = _offsets[edge1[0]];
							endP = _offsets[edge1[1]];
							// get absolut grid position of those points by adding current xy's
							startOffsetX = x + startP[0];
							startOffsetY = y + startP[1];
							endOffsetX = x + endP[0];
							endOffsetY = y + endP[1];
							// get field value for the start point
							v1 = _sliceData[startOffsetX][startOffsetY];
							// compute difference in field strength to end point
							dV = _sliceData[endOffsetX][endOffsetY] - v1;
							// get linear interpolation factor based difference in field strengths
							lerpF = (dV != 0) ? (isoV - v1) / dV : 0.5;
							// compute interpolated position of start vertex
							vx1 = n_w2 + (xscale * (startOffsetX + lerpF * (endOffsetX - startOffsetX)))-w2;
							vy1 = n_h2 + (yscale * (startOffsetY + lerpF * (endOffsetY - startOffsetY)));
							
							// now do the same thing for the end point of the line
							startP = _offsets[edge2[0]];
							endP = _offsets[edge2[1]];
							startOffsetX = x + startP[0];
							startOffsetY = y + startP[1];
							endOffsetX = x + endP[0];
							endOffsetY = y + endP[1];
							v1 = _sliceData[startOffsetX][startOffsetY];
							dV = _sliceData[endOffsetX][endOffsetY] - v1;
							lerpF = (dV != 0) ? (isoV - v1) / dV : 0.5;
							vx2 = n_w2 + (xscale * (startOffsetX + lerpF * (endOffsetX - startOffsetX)))-w2;
							vy2 = n_h2 + (yscale * (startOffsetY + lerpF * (endOffsetY - startOffsetY)));
							
							// add the render pipeline and vertex cache
							if (zSlice)
							{	
								d->lockWorldHash();
								
								d->world_hash[ voxel_id ][ "vertices" ][ vertex_counter ][ 0u ] = vx1;
								d->world_hash[ voxel_id ][ "vertices" ][ vertex_counter ][ 1u ] = vy1;
								d->world_hash[ voxel_id ][ "vertices" ][ vertex_counter ][ 2u ] = currSlice;
								
								d->world_hash[ voxel_id ][ "lines" ][ 0u ][ "indices" ][ line_counter ][ 0u ] = vertex_counter++;
								
								d->world_hash[ voxel_id ][ "vertices" ][ vertex_counter ][ 0u ] = vx2;
								d->world_hash[ voxel_id ][ "vertices" ][ vertex_counter ][ 1u ] = vy2;
								d->world_hash[ voxel_id ][ "vertices" ][ vertex_counter ][ 2u ] = currSlice;
																
								d->world_hash[ voxel_id ][ "lines" ][ 0u ][ "indices" ][ line_counter++ ][ 1u ] = vertex_counter++;
								
								d->unlockWorldHash();
							}
							else
							{
								d->lockWorldHash();
								
								d->world_hash[ voxel_id ][ "vertices" ][ vertex_counter ][ 0u ] = currSlice;
								d->world_hash[ voxel_id ][ "vertices" ][ vertex_counter ][ 1u ] = vy1;
								d->world_hash[ voxel_id ][ "vertices" ][ vertex_counter ][ 2u ] = vx1;
								
								d->world_hash[ voxel_id ][ "lines" ][ 0u ][ "indices" ][ line_counter ][ 0u ] = vertex_counter++;
								
								d->world_hash[ voxel_id ][ "vertices" ][ vertex_counter ][ 0u ] = currSlice ;
								d->world_hash[ voxel_id ][ "vertices" ][ vertex_counter ][ 1u ] = vy2;
								d->world_hash[ voxel_id ][ "vertices" ][ vertex_counter ][ 2u ] = vx2;
								
								d->world_hash[ voxel_id ][ "lines" ][ 0u ][ "indices" ][ line_counter++ ][ 1u ] = vertex_counter++;
								
								d->unlockWorldHash();
							}
							// set edge as already processed
							_edgeFlags[x][y] = true;
						} // end while currEdges[n] != -1
					} // end if corners > 0 && ...
				} // end if !_edgeFlags
				xx++;
			} // end for x = 0
		} // end for y = 0
		//endShape();
	} // end renderSlice()
	
	void VoxelSpace::lock( Device * d )
	{
		SDL_LockMutex( d->voxel_mutex );
	}
	
	void VoxelSpace::unlock( Device * d )
	{
		SDL_UnlockMutex( d->voxel_mutex );
		
	}

	
} // end namespace Kaleidoscope
