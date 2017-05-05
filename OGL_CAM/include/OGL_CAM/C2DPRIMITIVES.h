#ifndef C2DPRIMITIVES_H
#define C2DPRIMITIVES_H

#include "glut.h"

namespace OGLCAM
{

	class C2DPRIMITIVES
	{
	public:
		C2DPRIMITIVES(void){}
		~C2DPRIMITIVES(void){}

		void WriteInfo (GLfloat x, GLfloat y, char *format,...);
		void DrawReticles (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble HFov);
	};

} // Namespace
#endif // C2DPRIMITIVES_H
