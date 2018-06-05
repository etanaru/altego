cmake_minimum_required(VERSION 2.8.12)

project(altego)

find_package(dlib REQUIRED)

find_package(OpenCV REQUIRED COMPONENTS core videoio imgproc calib3d highgui)
include_directories(${OpenCV_INCLUDE_DIRS})

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")

add_executable(altego src/main.cpp src/window.cpp src/detection.cpp src/capture.cpp)
target_link_libraries(altego dlib::dlib ${OpenCV_LIBS})
