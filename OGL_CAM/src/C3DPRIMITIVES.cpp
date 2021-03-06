#include <math.h>
#include "OGL_CAM/C3DPRIMITIVES.h"
#include "OGL_CAM/3DVector.h"
#include "OGL_CAM/definitions.h"

namespace OGLCAM
{
	void C3DPRIMITIVES::drawCircleFAN(CWORLDCOORD center, GLfloat radius)
	{
		double twicePi = 2.0 * PI;
		GLfloat glx, gly, glz;
		center.getGLCoord3F (&glx, &gly, &glz);
		glColor3f(150.0f/255.0f, 218.0f/255.0f, 228.0f/255.0f);
		glLineWidth(1.0);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();
		glBegin(GL_LINES); 
		{
			for (int i = 0; i < 360; i++)   
			{
				glVertex3f(glx, gly, glz); // center of circle
				glNormal3f (0.0f, 1.0f, 0.0f);
				glVertex3f 
				(
					// Circle in x-z plane
					(glx + ((radius *World2GL) * cos(i * (twicePi / 360.0f)))),
					0.0f,
					(glz + ((radius *World2GL) * sin(i * (twicePi / 360.0f))))
				);
				glNormal3f (0.0f, 1.0f, 0.0f);
			}
		} glEnd(); //END GL_LINE_LOOP
	}

	void C3DPRIMITIVES::DrawCircle(CWORLDCOORD center, GLfloat radius, int num_segments, GLfloat* Color) 
	{
		GLfloat glx, gly, glz;
		center.getGLCoord3F (&glx, &gly, &glz);
		//glColor3f(150.0f/255.0f, 218.0f/255.0f, 228.0f/255.0f);
		glColor3f(*(Color), *(Color + 1), *(Color + 2));
		glLineWidth(1.0);
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();
		glBegin(GL_LINE_LOOP);
		{
			for (int i = 0; i < num_segments; i++)   
			{
				float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);//get the current angle 
				float x = (radius *World2GL) * cosf(theta);//calculate the x component 
				float z = (radius *World2GL) * sinf(theta);//calculate the y component 
				glVertex3f(glx + x , gly, glz + z);//output vertex 
				glNormal3f (0.0f, 1.0f, 0.0f);
			}
		}glEnd();
	}
} // Name space
