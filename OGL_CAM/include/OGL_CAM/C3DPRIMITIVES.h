#ifndef C3DPRIMITIVES_H
#define C3DPRIMITIVES_H

#include "glut.h"
#include "WORLDCOORD.h"

namespace OGLCAM
{

	class C3DPRIMITIVES
	{
	public:
		C3DPRIMITIVES(void){}
		~C3DPRIMITIVES(void){}

		void drawCircleFAN(CWORLDCOORD center, GLfloat radius);
		void DrawCircle(CWORLDCOORD center, GLfloat radius, int num_segments, GLfloat* Color) ; 
	};
} // Namespace
#endif // C3DPRIMITIVES_H
