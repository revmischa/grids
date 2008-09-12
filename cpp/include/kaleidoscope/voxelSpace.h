/*
 *  voxelSpace.h
 *  kaleidoscope_06
 *
 *  Created by Patrick Tierney on 9/11/08.
 *  Copyright 2008 Patrick Tierney. All rights reserved.
 *
 */


#pragma once

#include <kaleidoscope/define.h>
#include <kaleidoscope/device.h>

#include <vector>

namespace Kaleidoscope {
	
	class VoxelSpace {
		
	public:
		
		VoxelSpace( Grids::GridsID, int ww, int hh, int dd, int xs, int ys, int zs, int cacheSize);
		
		void setNodes( Device * d );
		void createPotentials( );
		
		void render();
		void update( Device *, float s, float isoV);
		
		void renderSlice( Device *, bool zSlice,float currSlice, float isoV);
		
		
		// space dimensions, the size over which to look when forming the "blob"
		int w, h, depth;
		// half width,height,depth
		int w2, h2, d2;
		// number of discrete steps along each axis
		int gridX, gridY, gridZ;
		// scale factor
		float xscale, yscale, zscale;
		// number of nodes used
		int numNodes;
		// current total power/potential of all nodes
		float totalP;
		
		// vertice cache
		float * cacheX;
		float * cacheY;
		float * cacheZ;
		
		// current list of nodes/voxels in the space
		// Node * nodes; // The nodes are equivalend to objects and people so I'll just use Vec3D of positions
		
		std::vector< Vec3D > node_positions;
		
		// vertex counter
		int vCount;
		
		Grids::GridsID voxel_id;
		
		
		
	private:
		
		std::vector< float > node_potentials;
		
		
		// internally used lookup tables by the marching square algorithm
		// see comments in renderSlice() method for more detail
		
		std::vector< std::vector< float > > _sliceData;
		std::vector< std::vector< bool > > _edgeFlags;
		
		std::vector< std::vector< int > > _edges;
		std::vector< std::vector< int > > _offsets;
		std::vector< std::vector< int > > _lines;
		
		
	}; // end class VoxelSpace
	
} // end namespace Kaleidoscope