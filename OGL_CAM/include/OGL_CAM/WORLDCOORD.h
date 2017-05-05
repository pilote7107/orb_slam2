#ifndef CWORLDCOORD_H
#define CWORLDCOORD_H


#include "glut.h"		// Need to include it here because the GL* types are required
#include "3DVector.h"

namespace OGLCAM
{
	const double World2GL = 0.001;

	class CWORLDCOORD
	{
	private :
		SF3dVector m_Position;
		double m_h;
		double m_p;
		double m_r;
	
	public:
		CWORLDCOORD(void);
		~CWORLDCOORD(void);

		void getGLCoord3F (GLfloat* glx, GLfloat* gly, GLfloat* glz);
		SF3dVector getGLCoordVector ();
		void getEuler3F   (GLfloat* h, GLfloat *p, GLfloat* r);
		void setCoordFromWorld (double x, double y, double z);
		void setCoordFromWorld (SF3dVector pos);
		void setEuler (double h, double p, double r);
		void rotateHeading (double angle) {m_h += angle;}
		void rotatePitch (double angle) {m_p += angle;}
		void rotateRoll (double angle) {m_r += angle;}
		double getHeading () {return m_h;}
		double getPitch () {return m_p;}
		double getRoll () {return m_r;}
	};


	} // Namespace
#endif // _CWORLDCOORD_H
