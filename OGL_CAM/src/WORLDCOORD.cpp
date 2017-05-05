#include "OGL_CAM/WORLDCOORD.h"


namespace OGLCAM
{
	CWORLDCOORD::CWORLDCOORD(void)
	{
		m_Position = F3dVector (0.0, 0.0, 0.0);
		m_h = m_p = m_r = 0.0;
	}

	CWORLDCOORD::~CWORLDCOORD(void)
	{
	}

	void CWORLDCOORD::getGLCoord3F (GLfloat* glx, GLfloat* gly, GLfloat* glz) 
	{
		*glx = (GLfloat)  m_Position.x * World2GL; 
		*gly = (GLfloat)  m_Position.z * World2GL; 
		*glz = (GLfloat) -m_Position.y * World2GL;
	}
	
	SF3dVector CWORLDCOORD::getGLCoordVector ()
	{
		SF3dVector tmp;
		tmp.x = m_Position.x * World2GL; 
		tmp.y = m_Position.z * World2GL; 
		tmp.z = -m_Position.y * World2GL;
		return tmp;
	}

	void CWORLDCOORD::getEuler3F   (GLfloat* h, GLfloat *p, GLfloat* r) 
	{
		*h = (GLfloat) m_h; 
		*p = (GLfloat) m_p; 
		*r = (GLfloat) m_r;
	}
	
	void CWORLDCOORD::setCoordFromWorld (double x, double y, double z) 
	{
		m_Position.x = x; 
		m_Position.y = y; 
		m_Position.z = z;
	}

	void CWORLDCOORD::setCoordFromWorld (SF3dVector pos) 
	{
		m_Position.x = pos.x; 
		m_Position.y = pos.y; 
		m_Position.z = pos.z;
	}
	
	void CWORLDCOORD::setEuler (double h, double p, double r) 
	{
		m_h = h; 
		m_p = p; 
		m_r = r;
	}
}
