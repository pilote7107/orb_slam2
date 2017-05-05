#include <math.h>
#include "OGL_CAM/3DVector.h"
#include "OGL_CAM/definitions.h"

namespace OGLCAM
{
	/////////////////////////////////
	//Note: All angles in degrees  //
	/////////////////////////////////
	
	SF3dVector F3dVector ( double x, double y, double z )
	{
		SF3dVector tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.z = z;
		return tmp;
	}

	double GetF3dVectorLength( SF3dVector * v)
	{
		return (sqrt(SQR(v->x)+SQR(v->y)+SQR(v->z)));
	}

	SF3dVector Normalize3dVector( SF3dVector v)
	{
		SF3dVector res;
		double l = GetF3dVectorLength(&v);
		if (l == 0.0f) return NULL_VECTOR;
		res.x = v.x / l;
		res.y = v.y / l;
		res.z = v.z / l;
		return res;
	}

	SF3dVector operator+ (SF3dVector v, SF3dVector u)
	{
		SF3dVector res;
		res.x = v.x+u.x;
		res.y = v.y+u.y;
		res.z = v.z+u.z;
		return res;
	}
	SF3dVector operator- (SF3dVector v, SF3dVector u)
	{
		SF3dVector res;
		res.x = v.x-u.x;
		res.y = v.y-u.y;
		res.z = v.z-u.z;
		return res;
	}

	SF3dVector operator* (SF3dVector v, float r)
	{
		SF3dVector res;
		res.x = v.x*r;
		res.y = v.y*r;
		res.z = v.z*r;
		return res;
	}

	SF3dVector CrossProduct (SF3dVector * u, SF3dVector * v)
	{
		SF3dVector resVector;
		resVector.x = u->y*v->z - u->z*v->y;
		resVector.y = u->z*v->x - u->x*v->z;
		resVector.z = u->x*v->y - u->y*v->x;

		return resVector;
	}

	double operator* (SF3dVector v, SF3dVector u)	//dot product
	{
		return v.x*u.x+v.y*u.y+v.z*u.z;
	}

} // Namespace
