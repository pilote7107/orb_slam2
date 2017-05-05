cmake_minimum_required(VERSION 3.0)
project(OGL_CAM)

set(CMAKE_CXX_FLAGS "-lGL")
set(CMAKE_CXX_FLAGS "-lglut")
set(CMAKE_CXX_FLAGS "-lGLU")
## Add support for C++11, supported in ROS Kinetic and newer

find_package(catkin REQUIRED COMPONENTS camera_info_manager image_transport nodelet roscpp sensor_msgs)

catkin_package(CATKIN_DEPENDS 
  roscpp
  sensor_msgs
  std_msgs
  imu_conversion
  INCLUDE_DIRS
  )

include_directories(${catkin_INCLUDE_DIRS}) 

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  sensor_msgs
  std_msgs
)
find_package (  
  GLUT REQUIRED
  )
find_package (  
  OpenGL REQUIRED
  )

set(SRCS_LIB
   /home/demo/catkin_orb2/src/OGL_CAM/src/3DVector.cpp
/home/demo/catkin_orb2/src/OGL_CAM/src/C2DPRIMITIVES.cpp
/home/demo/catkin_orb2/src/OGL_CAM/src/C3DOBJECT.cpp
/home/demo/catkin_orb2/src/OGL_CAM/src/C3DPRIMITIVES.cpp
/home/demo/catkin_orb2/src/OGL_CAM/src/C3DSCENE.cpp
/home/demo/catkin_orb2/src/OGL_CAM/src/OGLCAM.cpp
/home/demo/catkin_orb2/src/OGL_CAM/src/WORLDCOORD.cpp
    )
    
set(HEADERS_LIB
/home/demo/catkin_orb2/src/OGL_CAM/include/OGL_CAM/3DVector.h
/home/demo/catkin_orb2/src/OGL_CAM/include/OGL_CAM/C2DPRIMITIVES.h
/home/demo/catkin_orb2/src/OGL_CAM/include/OGL_CAM/C3DOBJECT.h
/home/demo/catkin_orb2/src/OGL_CAM/include/OGL_CAM/C3DPRIMITIVES.h
/home/demo/catkin_orb2/src/OGL_CAM/include/OGL_CAM/C3DSCENE.h
/home/demo/catkin_orb2/src/OGL_CAM/include/OGL_CAM/definitions.h
/home/demo/catkin_orb2/src/OGL_CAM/include/OGL_CAM/glut.h
/home/demo/catkin_orb2/src/OGL_CAM/include/OGL_CAM/OGLCAM.h
/home/demo/catkin_orb2/src/OGL_CAM/include/OGL_CAM/phidget21.h
/home/demo/catkin_orb2/src/OGL_CAM/include/OGL_CAM/WORLDCOORD.h
    )

add_executable(test1 src/test.cpp ${SRCS_LIB} ${HEADERS_LIB} )
add_dependencies(test1 ${catkin_EXPORTED_TARGETS})
add_executable(testPhidget1 src/testPhidget.cpp ${SRCS_LIB} ${HEADERS_LIB})
add_dependencies(test1 ${catkin_EXPORTED_TARGETS})

include_directories(${GLUT_INCLUDE_DIRS} ${OpenGL_INCLUDE_DIRS} include)

target_link_libraries(test1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${catkin_LIBRARIES} ) 

include_directories(${GLUT_INCLUDE_DIRS} ${OpenGL_INCLUDE_DIRS} include)
target_link_libraries(testPhidget1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${catkin_LIBRARIES} -lglut phidget21) 




