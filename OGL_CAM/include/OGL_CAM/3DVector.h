#ifndef _3DVECTOR_H
#define _3DVECTOR_H


namespace OGLCAM
{
	/////////////////////////////////
	//Note: All angles in degrees  //
	/////////////////////////////////
	struct SF3dVector  //Float 3d-vect, normally used
	{
		double x,y,z;
	};
	
	extern SF3dVector F3dVector ( double x, double y, double z );
	extern double GetF3dVectorLength( SF3dVector * v);
	extern SF3dVector Normalize3dVector( SF3dVector v);
	extern SF3dVector operator+ (SF3dVector v, SF3dVector u);
	extern SF3dVector operator- (SF3dVector v, SF3dVector u);
	extern SF3dVector operator* (SF3dVector v, float r);
	extern SF3dVector CrossProduct (SF3dVector * u, SF3dVector * v);
	extern double operator* (SF3dVector v, SF3dVector u);	//dot product

} // Namespace
#endif // _3DVECTOR_H