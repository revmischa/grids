



#include <kaleidoscope/geo.h>


namespace Kaleidoscope
{
	Geo::Geo(){
	}
	
	float Geo::distanceToSphereAlongRay( Vec3D ray_pos, Vec3D ray_tar, Vec3D sphere_pos, float sphere_radius )
	{
		Vec3D q = sphere_pos - ray_pos;
		float c = q.getLength();
		float v = q.dotProduct(ray_tar);
		float d = sphere_radius * sphere_radius - (c * c - v * v);
		
		// If there was no intersection, return -1
		if (d < 0.0f)
			return -1;
		
		// Return the distance to the [first] intersecting point
		return v - (float) sqrt(d);             
		
	}
} // end namespace Kaleidoscope
