/**
* This file is part of ORB-SLAM2.
*
* Copyright (C) 2014-2016 Raúl Mur-Artal <raulmur at unizar dot es> (University of Zaragoza)
* For more information see <https://github.com/raulmur/ORB_SLAM2>
*
* ORB-SLAM2 is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* ORB-SLAM2 is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with ORB-SLAM2. If not, see <http://www.gnu.org/licenses/>.
*/


#include<iostream>
#include<algorithm>
#include<fstream>
#include<chrono>
#include <cstdlib>

#include<ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include "/home/demo/catkin_orb2/devel/include/imu_conversion/Eulers.h"
#include<opencv2/core/core.hpp>
#include "tf/transform_datatypes.h"
#include <tf/transform_broadcaster.h>

#include"../../../include/System.h"

using namespace std;

vector <double>Roll_Vector; 
vector <double>Pitch_Vector; 
vector <double>Yaw_Vector;
  double Pitch_Array[30] = {};
  double Roll_Array[30] = {};

class ImageGrabber
{
public:
    ImageGrabber(ORB_SLAM2::System* pSLAM,ros::NodeHandle nh):mpSLAM(pSLAM)
    {
	orientation_publisher = nh.advertise<imu_conversion::Eulers>("/imu/data/euler", 5);

	//vector <double>Roll_Vector = vector <double>(100);
	//vector <double>Pitch_Vector = vector <double>(100);
	//vector <double>Yaw_Vector = vector <double>(100);
	

	ROS_INFO("***constructor before for Roll_Vector size = %i*** ",Roll_Vector.size());

    }

    void GrabStereo(const sensor_msgs::ImageConstPtr& msgLeft,const sensor_msgs::ImageConstPtr& msgRight);

    ORB_SLAM2::System* mpSLAM;
    bool do_rectify;
    cv::Mat M1l,M2l,M1r,M2r;
    imu_conversion::Eulers orientation;
    ros::Publisher orientation_publisher;
    //vector <double>Roll_Vector;
    //vector <double>Pitch_Vector;
    //vector <double>Yaw_Vector;

};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "RGBD");
    ros::NodeHandle nh;
    ros::start();
    

    if(argc != 7)
    {
        cerr << endl << "Usage: rosrun ORB_SLAM2 Stereo path_to_vocabulary path_to_settings do_rectify size_roll_buffer size_pitch_buffer size_yaw_buffer" << endl;
        ros::shutdown();
        return 1;
    }    

    // Create SLAM system. It initializes all system threads and gets ready to process frames.
    ORB_SLAM2::System SLAM(argv[1],argv[2],ORB_SLAM2::System::STEREO,true);

    ImageGrabber igb (&SLAM,nh);

     
     Roll_Vector.reserve(30);
     Pitch_Vector.reserve(30);
     Yaw_Vector.reserve(30);

     Roll_Vector= vector <double>(30);
     Pitch_Vector= vector <double>(30);
     Yaw_Vector= vector <double>(30);



    Roll_Vector.resize(atoi(argv[4]));
    Pitch_Vector.resize(atoi(argv[5]));
    Yaw_Vector.resize(atoi(argv[6]));

     stringstream ss(argv[3]);
	ss >> boolalpha >> igb.do_rectify;


    if(igb.do_rectify)
    {      
        // Load settings related to stereo calibration
        cv::FileStorage fsSettings(argv[2], cv::FileStorage::READ);
        if(!fsSettings.isOpened())
        {
            cerr << "ERROR: Wrong path to settings" << endl;
            return -1;
        }

        cv::Mat K_l, K_r, P_l, P_r, R_l, R_r, D_l, D_r;
        fsSettings["LEFT.K"] >> K_l;
        fsSettings["RIGHT.K"] >> K_r;

        fsSettings["LEFT.P"] >> P_l;
        fsSettings["RIGHT.P"] >> P_r;

        fsSettings["LEFT.R"] >> R_l;
        fsSettings["RIGHT.R"] >> R_r;

        fsSettings["LEFT.D"] >> D_l;
        fsSettings["RIGHT.D"] >> D_r;

        int rows_l = fsSettings["LEFT.height"];
        int cols_l = fsSettings["LEFT.width"];
        int rows_r = fsSettings["RIGHT.height"];
        int cols_r = fsSettings["RIGHT.width"];

        if(K_l.empty() || K_r.empty() || P_l.empty() || P_r.empty() || R_l.empty() || R_r.empty() || D_l.empty() || D_r.empty() ||
                rows_l==0 || rows_r==0 || cols_l==0 || cols_r==0)
        {
            cerr << "ERROR: Calibration parameters to rectify stereo are missing!" << endl;
            return -1;
        }

        cv::initUndistortRectifyMap(K_l,D_l,R_l,P_l.rowRange(0,3).colRange(0,3),cv::Size(cols_l,rows_l),CV_32F,igb.M1l,igb.M2l);
        cv::initUndistortRectifyMap(K_r,D_r,R_r,P_r.rowRange(0,3).colRange(0,3),cv::Size(cols_r,rows_r),CV_32F,igb.M1r,igb.M2r);
    }



    message_filters::Subscriber<sensor_msgs::Image> left_sub(nh, "/stereo/left/image_rect", 1);
    message_filters::Subscriber<sensor_msgs::Image> right_sub(nh, "/stereo/right/image_rect", 1);
    typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> sync_pol;
    message_filters::Synchronizer<sync_pol> sync(sync_pol(10), left_sub,right_sub);
    sync.registerCallback(boost::bind(&ImageGrabber::GrabStereo,&igb,_1,_2));

    ros::spin();

    // Stop all threads
    SLAM.Shutdown();

    // Save camera trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory_TUM_Format.txt");
    SLAM.SaveTrajectoryTUM("FrameTrajectory_TUM_Format.txt");
    SLAM.SaveTrajectoryKITTI("FrameTrajectory_KITTI_Format.txt");

    ros::shutdown();

    return 0;
}

void ImageGrabber::GrabStereo(const sensor_msgs::ImageConstPtr& msgLeft,const sensor_msgs::ImageConstPtr& msgRight)
{
    // Copy the ros image message to cv::Mat.
    cv_bridge::CvImageConstPtr cv_ptrLeft;
    try
    {
        cv_ptrLeft = cv_bridge::toCvShare(msgLeft);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv_bridge::CvImageConstPtr cv_ptrRight;
    try
    {
        cv_ptrRight = cv_bridge::toCvShare(msgRight);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    if(do_rectify)
    {
        cv::Mat imLeft, imRight;
        cv::remap(cv_ptrLeft->image,imLeft,M1l,M2l,cv::INTER_LINEAR);
        cv::remap(cv_ptrRight->image,imRight,M1r,M2r,cv::INTER_LINEAR);
        mpSLAM->TrackStereo(imLeft,imRight,cv_ptrLeft->header.stamp.toSec());
    }
    else
    {
         cv::Mat pose = mpSLAM->TrackStereo(cv_ptrLeft->image,cv_ptrRight->image,cv_ptrLeft->header.stamp.toSec());



if (pose.empty())
       return;

       /* global left handed coordinate system */
       static cv::Mat pose_prev = cv::Mat::eye(4,4, CV_32F);
       static cv::Mat world_lh = cv::Mat::eye(4,4, CV_32F);
       // matrix to flip signs of sinus in rotation matrix, not sure why we need to do that
       static const cv::Mat flipSign = (cv::Mat_<float>(4,4) <<   1,-1,-1, 1,
                                                                  -1, 1,-1, 1,
                                                                  -1,-1, 1, 1,
                                                                   1, 1, 1, 1);

       //prev_pose * T = pose
       cv::Mat translation =  (pose * pose_prev.inv());//.mul(flipSign);
       world_lh = world_lh * translation;
       pose_prev = pose.clone();


       /* transform into global right handed coordinate system, publish in ROS*/
       tf::Matrix3x3 cameraRotation_rh(  - world_lh.at<float>(0,0),   world_lh.at<float>(0,1),   world_lh.at<float>(0,2),
                                     - world_lh.at<float>(1,0),   world_lh.at<float>(1,1),   world_lh.at<float>(1,2),
                                       world_lh.at<float>(2,0), - world_lh.at<float>(2,1), - world_lh.at<float>(2,2));

       tf::Vector3 cameraTranslation_rh( world_lh.at<float>(0,3),world_lh.at<float>(1,3), - world_lh.at<float>(2,3) );

       //rotate 270deg about x and 270deg about x to get ENU: x forward, y left, z up
       const tf::Matrix3x3 rotation270degXZ(   0, 1, 0,
                                               0, 0, 1,
                                               1, 0, 0);

       const tf::Matrix3x3 rotation180degY(   -1, 0, 0,
                                              0,  1, 0,
                                              0,  0, -1);

       static tf::TransformBroadcaster br;

       tf::Matrix3x3 globalRotation_rh = cameraRotation_rh*rotation180degY;//*rotation270degXZ*rotation90degY ;
       tf::Vector3 globalTranslation_rh = cameraTranslation_rh*rotation180degY;//* rotation270degXZ*rotation90degY;
       tf::Transform transform = tf::Transform(globalRotation_rh, globalTranslation_rh);


   double roll_true, pitch_true, yaw_true=0;
   double sum_roll, sum_pitch, sum_yaw = 0;

   tf::Matrix3x3(transform.getRotation()).getRPY(roll_true, pitch_true, yaw_true);

   ROS_ERROR("ROLL: %f, PITCH: %f, YAW: %f", yaw_true*180/M_PI,roll_true*180/M_PI,pitch_true*180/M_PI);

   transform.setRotation(tf::createQuaternionFromRPY(yaw_true, roll_true, pitch_true));

   br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "camera_link", "camera_pose"));

ROS_INFO("***before for Roll_Vector SIZE = %d*** ",Roll_Vector.size());
ROS_INFO("***before for Pitch_Vector SIZE = %d*** ",Pitch_Vector.size());
ROS_INFO("***before for Yaw_Vector SIZE = %d*** ",Yaw_Vector.size());

	for(int i=0;i<Pitch_Vector.size()-1;i++)
	{ 
		sum_roll = sum_roll +Roll_Vector.at(i);
		//sum_roll = sum_roll +Roll_Array[i];

	        sum_pitch = sum_pitch + Pitch_Array[i];
		//sum_pitch = sum_pitch +Pitch_Vector.at(i);

		sum_yaw = sum_yaw 
		//sum_yaw = sum_yaw +Yaw_Vector.at(i);
		
		//Roll_Array[i] = Roll_Array[i+1];
     		Roll_Vector.at(i) = Roll_Vector.at(i+1);
		Pitch_Array[i] = Pitch_Array[i+1];
		//Pitch_Vector.at(i) = Pitch_Vector.at(i+1);
		//Yaw_Vector.at(i) = Yaw_Vector.at(i+1);
		
		
	}
   
ROS_INFO("***add to Pitch r value*** ");
    Roll_Vector.at(Roll_Vector.size()-1)=roll_true;
    //Roll_Array[29] =roll_true;
    //Pitch_Vector.at(Roll_Vector.size()-1)=yaw_true;
    Pitch_Array[29]=yaw_true;

    Yaw_Vector.at(Roll_Vector.size()-1)=- pitch_true;

   sum_roll +=  Roll_Vector.at(Roll_Vector.size()-1);
   //sum_roll +=  Roll_Array[29];
   sum_pitch += Pitch_Array[29];
   //sum_yaw +=   Yaw_Vector.at(Yaw_Vector.size()-1);
sum_yaw += 0;
   

ROS_INFO("***before meaning*** ");

   orientation.roll = sum_roll*180/M_PI/30;
   orientation.pitch = sum_pitch*180/M_PI/30;
   orientation.yaw =  sum_yaw*180/M_PI/30;
   orientation_publisher.publish (orientation);

    }

}



