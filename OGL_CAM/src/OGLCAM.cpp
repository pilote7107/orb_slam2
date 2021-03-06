#include <math.h>
#include "OGL_CAM/OGLCAM.h"
#include "OGL_CAM/definitions.h"


namespace OGLCAM
{
	GLfloat HFov = DEFAULT_FOV;
	GLfloat NCP  = DEFAULT_NCP;
	GLfloat FCP  = DEFAULT_FCP;
	GLfloat Heading = 0.0; // Rotation selon l'axe Z Sens trigo : n�gatif vers la droite positif vers la gauche
	GLfloat Pitch = 0.0;   // Rotation selon l'axe X Sens trigo : n�gatif � piquer positif � cabrer
	GLfloat Roll = 0.0;	   // Rotation selon l'axe Y Sens trigo : n�gatif vers la gauche positif vers la droite
	GLfloat PosX = 0.0;
	GLfloat PosY = 0.0;
	GLfloat PosZ = DEFAULT_CAMERA_HEIGHT;

	COGLCAM::COGLCAM (void)
	{
		m_Width = DEFAULTWINDOW_WIDTH;
		m_Height = DEFAULTWINDOW_HEIGHT;
		m_HFov = DEFAULT_FOV;
		m_NCP = DEFAULT_NCP;
		m_FCP = DEFAULT_FCP;
		
		ResetAngles ();
		
		m_scene.InitializeRC ();
		m_scene.load();
	}

	COGLCAM::~COGLCAM(void)
	{
	}

	void COGLCAM::ResetAngles ()
	{
		GLfloat glx, gly, glz;
		m_Position.getGLCoord3F (&glx, &gly, &glz);
		m_ViewDir = F3dVector( glx, gly, glz-1.0);
		m_RightVector = F3dVector (glx + 1.0f, gly, glz);
		m_UpVector = F3dVector (glx, gly + 1.0f, glz);
		m_Position.setEuler (0.0, 0.0, 0.0);
	}
	
	void COGLCAM::Update (int Width, int Height, GLfloat HFov, GLfloat NCP, GLfloat FCP)
	{
		if (Width == 0 || Height == 0) return;  //Nothing is visible then, so return
		if (HFov < 1.0) return;

		m_Width = Width;
		m_Height = Height;
		m_HFov = HFov;
		m_NCP = NCP;
		m_FCP = FCP;

		glViewport(0,0,m_Width,m_Height);  //Use the whole window for rendering	

	}
 
	void COGLCAM::Update (GLfloat x, GLfloat y, GLfloat z, GLfloat h, GLfloat p, GLfloat r, GLfloat HFov)
	{
		// Update Position
		SetPos (x, y, z);
		
		// Apply Euler
		ResetAngles ();
		RotateYaw (h);
		RotatePitch (p);
		RotateRoll (-r);
		
		// Update horizontal FOV
		m_HFov = HFov;
	}

	void COGLCAM::SetPos (SF3dVector pos)
	{
		m_Position.setCoordFromWorld (pos);
	}

	void COGLCAM::SetPos (double x, double y, double z)
	{
		SF3dVector pos = F3dVector(x, y, z);
		SetPos (pos);
	}

	void COGLCAM::RotatePitch (GLfloat Angle)
	{
		m_Position.rotatePitch (Angle);
		
		//Rotate viewdir around the right vector:
		m_ViewDir = Normalize3dVector(m_ViewDir*cos(Angle*PIdiv180) + m_UpVector*sin(Angle*PIdiv180));

		//now compute the new UpVector (by cross product)
		m_UpVector = CrossProduct(&m_ViewDir, &m_RightVector)*-1;
	}

	void COGLCAM::RotateYaw (GLfloat Angle)
	{
		m_Position.rotateHeading (Angle);
		
		//Rotate viewdir around the up vector:
		m_ViewDir = Normalize3dVector(m_ViewDir*cos(Angle*PIdiv180) - m_RightVector*sin(Angle*PIdiv180));

		//now compute the new RightVector (by cross product)
		m_RightVector = CrossProduct(&m_ViewDir, &m_UpVector);
	}

	void COGLCAM::RotateRoll (GLfloat Angle)
	{
		m_Position.rotateRoll (Angle);
		
		//Rotate m_RightVector around the view vector:
		m_RightVector = Normalize3dVector(m_RightVector*cos(Angle*PIdiv180) + m_UpVector*sin(Angle*PIdiv180));

		//now compute the new UpVector (by cross product)
		m_UpVector = CrossProduct(&m_ViewDir, &m_RightVector)*-1;
	}

	void COGLCAM::PreDraw ()
	{
		//Set a new projection matrix
		glEnable (GL_LIGHTING);
		glEnable (GL_LIGHT0);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_NORMALIZE);
		glLineWidth(1.0);
		glDisable (GL_FOG);
		glEnable (GL_DEPTH_TEST);    
		glEnable (GL_CULL_FACE);

		// perspective
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		gluPerspective
			(
				2.0 * atan (tan(m_HFov * 0.5 * PIdiv180) / ((GLdouble)m_Width/(GLdouble)m_Height)) / PIdiv180,
				(GLdouble)m_Width/(GLdouble)m_Height, 
				m_NCP * World2GL, 
				m_FCP * World2GL
			);


		//The point at which the camera looks:
		//glMatrixMode (GL_MODELVIEW);
		//glLoadIdentity ();
		SF3dVector Position  = m_Position.getGLCoordVector ();
		SF3dVector ViewPoint = Position + m_ViewDir;
		gluLookAt
		(	
			Position.x,
			Position.y,
			Position.z,
			ViewPoint.x,
			ViewPoint.y,
			ViewPoint.z,
			m_UpVector.x,
			m_UpVector.y,
			m_UpVector.z
		);
		glClearColor(150.0f/255.0f, 195.0f/255.0f, 228.0f/255.0f, 0.0f); // Sky color
	    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void COGLCAM::Draw( void )
	{	
		// Draw Ground
		glDisable(GL_LIGHTING);
		CWORLDCOORD center;
		GLfloat Green [3] = {150.0f/255.0f, 218.0f/255.0f, 228.0f/255.0f};
		m_3DPrimitives.drawCircleFAN(center, 2560.0);
		m_3DPrimitives.DrawCircle(center, 10.0f, 360.0f, (GLfloat*) &Green); 
		m_3DPrimitives.DrawCircle(center, 20.0f, 360.0f, (GLfloat*) &Green);
		m_3DPrimitives.DrawCircle(center, 40.0f, 360.0f, (GLfloat*) &Green);
		m_3DPrimitives.DrawCircle(center, 80.0f, 360.0f, (GLfloat*) &Green);
		m_3DPrimitives.DrawCircle(center, 160.0f, 360.0f,(GLfloat*) &Green);
		m_3DPrimitives.DrawCircle(center, 320.0, 360.0f, (GLfloat*) &Green); 
		m_3DPrimitives.DrawCircle(center, 640.0, 360.0f, (GLfloat*) &Green); 
		m_3DPrimitives.DrawCircle(center, 1280.0, 360.0f,(GLfloat*) &Green);
		m_3DPrimitives.DrawCircle(center, 2560.0, 360.0f,(GLfloat*) &Green); 
		GLfloat Red [3] = {218.0f/255.0f, 150.0f/255.0f, 228.0f/255.0f};
		m_3DPrimitives.DrawCircle(center, 500, 360.0f,(GLfloat*) &Red);
		m_3DPrimitives.DrawCircle(center, 1500.0, 360.0f,(GLfloat*) &Red); 



		// Draw scene
		glEnable(GL_LIGHTING);
		m_scene.draw ();
	}

	void COGLCAM::PostDraw( void )
	{
		// Screen Coordinate
		GLdouble left   =  0; 
		GLdouble right  =  m_Width * 5; 
		GLdouble bottom =  0; 
		GLdouble top    =  m_Height * 5;

		// Overlays
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(left, right, bottom, top);
		glMatrixMode(GL_MODELVIEW);
		m_2DPrimitives.WriteInfo (10, top -150, "heading: %00.03f / pitch: %00.03f / roll: %00.03f / HFov: %00.02f", m_Position.getHeading (), m_Position.getPitch (), m_Position.getRoll (), m_HFov);
		m_2DPrimitives.DrawReticles (left, right, bottom,top, m_HFov);

		//finish rendering:
		glutSwapBuffers();
	}

} // Namespace
