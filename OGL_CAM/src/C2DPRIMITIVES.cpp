#include <stdio.h>
#include <stdarg.h>
#include "OGL_CAM/C2DPRIMITIVES.h"

namespace OGLCAM
{
	void C2DPRIMITIVES::WriteInfo (GLfloat x, GLfloat y, char *format,...)
	{
	  va_list args;
	  char buffer[200], *p;

	  va_start(args, format);
	  //vsnprintf (buffer, format, args);
	  va_end(args);
	  glColor3f(65.0f/255.0f, 100.0f/255.0f, 250.0f/255.0f);
	  glLineWidth(1.0);
	  glPushMatrix();
	  glLoadIdentity();
	  glPushMatrix();
	  glTranslatef(x, y, 0);
	  for (p = buffer; *p; p++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	  glPopMatrix();
	}

	void C2DPRIMITIVES::DrawReticles (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble HFov)
	{
		GLfloat GraduationPeriod = (right - left) / HFov;
		GLfloat GraduationSize = (top - bottom) / 100.0f;
		GLfloat MiddleScreenX = (right - left) / 2.0;
		GLfloat MiddleScreenY = (top - bottom) / 2.0;
		GLfloat GraduationScale = 1.0;
		glColor3f(128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0);
		glLineWidth(1.0);
		glPushMatrix();
		glLoadIdentity();
		glPushMatrix();
		glBegin(GL_LINES);
		{
			glVertex2f(	
							left,
							MiddleScreenY
					  );
			glVertex2f(	
							right,
							MiddleScreenY
					  );
			glVertex2f(	
							MiddleScreenX,
							top
					  );
			glVertex2f(	
							MiddleScreenX,
							bottom
					  );
			//  graduations
			for (int i = 1; i <= 10; i++)
			{
				if ((i % 10) == 0) GraduationScale = 2.0;
				else if ((i % 5) == 0) GraduationScale = 1.5;
				else GraduationScale = 1.0;
				glVertex2f(	
							MiddleScreenX + (i * GraduationPeriod),
							MiddleScreenY + (GraduationSize * GraduationScale)
					  );
				glVertex2f(	
							MiddleScreenX + (i * GraduationPeriod),
							MiddleScreenY - (GraduationSize * GraduationScale)
					  );
				glVertex2f(	
							MiddleScreenX - (i * GraduationPeriod),
							MiddleScreenY + (GraduationSize * GraduationScale)
					  );
				glVertex2f(	
							MiddleScreenX - (i * GraduationPeriod),
							MiddleScreenY - (GraduationSize * GraduationScale)
					  );
				glVertex2f(	
							MiddleScreenX + (GraduationSize * GraduationScale),
							MiddleScreenY + (i * GraduationPeriod)
					  );
				glVertex2f(	
							MiddleScreenX - (GraduationSize * GraduationScale),
							MiddleScreenY + (i * GraduationPeriod)
					  );
				glVertex2f(	
							MiddleScreenX + (GraduationSize * GraduationScale),
							MiddleScreenY - (i * GraduationPeriod)
					  );
				glVertex2f(	
							MiddleScreenX - (GraduationSize * GraduationScale),
							MiddleScreenY - (i * GraduationPeriod)
					  );
			}
		} glEnd();
	}
} // namespace
