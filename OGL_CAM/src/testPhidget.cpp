#include "ros/ros.h"
#include <iostream>
#include <phidget21.h>
#include "OGL_CAM/glut.h"
#include "OGL_CAM/OGLCAM.h"
#include "OGL_CAM/definitions.h"
#include <sensor_msgs/Imu.h>

using namespace OGLCAM;

COGLCAM Camera;
CPhidgetSpatial_SpatialEventData SensorData;
CPhidgetSpatial_SpatialEventData CalibrationData;

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

	 // sensor calibration 
	for (int j = 0; j < 3; j++)
	{
		CalibrationData.acceleration   [j] = SensorData.acceleration  [j];
		CalibrationData.angularRate    [j] = SensorData.angularRate   [j];
		CalibrationData.magneticField  [j] = SensorData.magneticField [j];
	}
	// Reset common data structure
	SensorData.acceleration [0] = SensorData.acceleration [1] = SensorData.acceleration [2] = 0.0;
	SensorData.angularRate  [0] = SensorData.angularRate  [1] = SensorData.angularRate  [2] = 0.0;
	SensorData.magneticField[0] = SensorData.magneticField[1] = SensorData.magneticField[2] = 0.0;
	SensorData.timestamp.seconds = SensorData.timestamp.microseconds = 0.0;
}

//handles the window's resizing event
void reshape(int x, int y)
{
	Camera.Update (x, y, HFov, NCP, FCP);
}

//displays the scene.
void Display(void)
{
	Camera.Update (PosX, PosY, PosZ, Heading, Pitch, Roll, HFov);
	Camera.Render ();
}

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
}

//callback that will run if the Spatial is attached to the computer

void IdleFunc() 
{
	// Update camera with magentometer data
	Pitch = (GLfloat) SensorData.magneticField[0];
	Roll = (GLfloat) SensorData.magneticField[1];
	Heading = (GLfloat) SensorData.magneticField[2];


	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	int result = 0;

	std::cout << "---------------------------------------------"<< std::endl;
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

	
	// Window creation (GLUT)
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(DEFAULTWINDOW_WIDTH,DEFAULTWINDOW_HEIGHT);
	glutCreateWindow("Camera");
	glutDisplayFunc(Display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(KeyDown);
	
	//OPEN GL Call backs registration
	//glutIdleFunc(IdleFunc);

	// Camera reset for calibration
	CameraReset ();

	// Main simulation loop
	glutMainLoop();


	return result;             
}
