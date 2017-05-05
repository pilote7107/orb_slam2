#include <iostream>
#include "OGL_CAM/glut.h"
#include "OGL_CAM/OGLCAM.h"
#include <sensor_msgs/Imu.h>
#include <iostream>
#include <string>
#include <cstdio>
#include <unistd.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "/home/demo/catkin_orb2/devel/include/imu_conversion/Eulers.h"

using namespace OGLCAM;

COGLCAM Camera;

float rollArray[100] = {};
float pitchArray[100] = {};
float yawArray[100] = {};


// Resets the camera
void CameraReset ()
{
	 HFov = DEFAULT_FOV;
	 NCP  = DEFAULT_NCP;
	 FCP  = DEFAULT_FCP;
	 Heading = 0.0;
	 Pitch = 0.0;
	 Roll = 0.0;
	 PosX = 0.0;
	 PosY = 0.0;
	 PosZ = DEFAULT_CAMERA_HEIGHT;
}

//handles the window's resizing event
void reshape(int x, int y)
{
	Camera.Update (x, y, HFov, NCP, FCP);
}

//displays the scene.


//handles the key down events
void KeyDown(unsigned char key, int x, int y)
{
	switch (key) 
	{
		case '6':		
			Heading-=0.5;
			break;
		case '4':		
			Heading+=0.5;;
			break;
		case '8':		
			Pitch -= 0.5;
			break;
		case '2':		
			Pitch += 0.5;
			break;
		case '1':		
			Roll -= 0.5;
			break;
		case '3':		
			Roll += 0.5;
			break;
		case '+':
			if (HFov >= 2.0) HFov -= 1.0f;
			break;
		case '-':		
			if (HFov < 179.0) HFov += 1.0f;
			break;
		case '7':
			CameraReset ();
			break;
	}
	glutPostRedisplay();
}

void spinner()
{	
	ros::spinOnce();
}

void printtext(int x, int y, std::string String)
{
//(x,y) is from the bottom left of the window
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, DEFAULTWINDOW_WIDTH, 0, DEFAULTWINDOW_HEIGHT, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2i(x,y);
   
    for (int i=0; i<String.size(); i++)
    {
        glutBitmapCharacter(  GLUT_BITMAP_TIMES_ROMAN_24  , String[i]);
    }
    
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
void ImuCallback(const imu_conversion::Eulers::ConstPtr& msg)
{
	/*
	// Update camera with magentometer data
	float sum_roll,sum_pitch,sum_yaw =0;
	int i =0;

	for(i=0;i<99;i++)
	{ 
		sum_roll += rollArray[i];
		sum_pitch += pitchArray[i];
		sum_yaw += yawArray[i];

     		rollArray[i] = rollArray[i+1];
		pitchArray[i] = pitchArray[i+1];
		yawArray[i] = yawArray[i+1];
	}
	
	rollArray[99] = msg-> roll;
	pitchArray[99] = msg->pitch;
	yawArray[99] = msg->yaw;

	sum_roll += rollArray[99];
	sum_pitch += pitchArray[99];
	sum_yaw += yawArray[99];

	Roll = sum_roll/100.0  ;
	Pitch= sum_pitch/100.0 ;
	Heading = sum_yaw/100.0 ;

	*/
	
	Roll = msg-> roll;
	Pitch = msg->pitch;
	Heading = msg->yaw;	
	
	
}
void Display(void)
{
	std::ostringstream os;
	Camera.Update (PosX, PosY, PosZ, Heading, Pitch, Roll, HFov);
	Camera.Render ();
	os<<"Roll=";
	os<<Roll;
	os<<"  Pitch=";
	os<<Pitch;
	os<<"  Yaw=";
	os<<Heading;
 	printtext(10,10,os.str());
	glutSwapBuffers();
	glutPostRedisplay();

}
int main(int argc, char **argv)
{
	ros::init(argc, argv, "OGL_interface_node");
  	


	std::cout << "Camera simulation for traking test\n";
	std::cout << "6 rotate camera right 0.5 deg\n";
	std::cout << "4 rotate camera left  0.5 deg\n";
	std::cout << "2 rotate camera up    0.5 deg\n";
	std::cout << "8 rotate camera down  0.5 deg\n";
	std::cout << "1 Roll camera left    0.5 deg\n";
	std::cout << "3 Roll camera right   0.5 deg\n";
	std::cout << "+ zoom in             1 deg\n";
	std::cout << "- zoom out            1 deg\n";
	std::cout << "7 RESET VIEW \n";
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(DEFAULTWINDOW_WIDTH,DEFAULTWINDOW_HEIGHT);
	glutCreateWindow("Camera");
	glutDisplayFunc(Display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(KeyDown);
	//glutTimerFunc(10,spinner,0);
	ros::NodeHandle n;

	//ros::Subscriber sub = n.subscribe("/imu_bno/data_raw/euler", 10, ImuCallback);
	ros::Subscriber sub = n.subscribe("/imu/data/euler", 10, ImuCallback);
	glutIdleFunc(spinner);
	glutMainLoop();


	return 0;  
}
