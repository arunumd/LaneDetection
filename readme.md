# Lane detection

[![Build Status](https://travis-ci.org/arunumd/LaneDetection.svg?branch=master)](https://travis-ci.org/arunumd/LaneDetection)
[![Coverage Status](https://coveralls.io/repos/github/arunumd/LaneDetection/badge.svg?branch=master)](https://coveralls.io/github/arunumd/LaneDetection?branch=master)
---

## Overview

Simple starter C++ project with:

- cmake
- googletest
- OpenCV
- boost cpp library

## Project purpose

Lane detection is an active area of research in autonomous driving field. The ability of an autonomous vehicle to detect the road and track it continuously is at the heart of autonomous driving. There are several approaches to lane detection like the usage of deep learning, computer vision, machine learning, etc.

Every method has its own advantages and disadvantages. Neural networks are supposed to be the most powerful technique for lane detection. This project makes use of an image segmentation based approach. The most distinguishing feature of lanes on roads are the white or the yellow colored lane markings on the road surface. There isn't much variation in the lane markings on different roads. Hence such lane markings can be considered as reliable approach to lane detection. This project makes use of the computer vision tools provided by the OpenCV library and does image based segmentation for detecting lanes.

Segmentation is the process of isolating colors of interest from the remaining details present in a scene (image). For any segmentation we apply some kind of thresholding operation based on our colors of interest. In this project, we apply thresholds for white and yellow colors in Lab color space. While there are several other color spaces like HSV, HSL, RGB, BGR, etc., Lab color space is quite unique in that it has a separate channel dedicated only for lightness and darkness of an image. The remaining two channels are for the colors of the image. Hence this color space gives an operational ease to the user and gives more control to the user during thresholding. 

## Updates with GMock Testing

In further to the original project scope, the Thresholder class is now mocked with Google's GMock testing framework inside /test/test.cpp file. A new class called MockThresholder which inherits all the properties and methods of Thresholder class has been created. The mocked class is expected to call the mocked methods sequentially and then the actual returned types are verified with expected return types.

**The image shown below is the result obtained from Lab color thresholding operation**
![LabThreshold](http://i63.tinypic.com/ogcuhw.png)

After segmentation of the white and yellow lanes, we obtain a binary image wherein the regions of the image with the lanes become ones and all other regions in the image become zeros. After this step, the process is quite easy. We apply a gradient based edge detector like the Canny edge detector and then we apply Hough transform to obtain Hough lines on top of the edges. 

**A sample screenshot of the canny edge detection for this project is shown below**
![CannyOutput](http://i64.tinypic.com/25gdzd3.png)


**The image shown below is a sample screenshot of HoughLines detection**
![HoughDetection](http://i63.tinypic.com/2qb7tki.png)

Later, we manipulate the slopes of the obtained Hough lines and mark the lanes. We also make announcements like the road conditions ahead (left turn, right turn, etc.).

**The following image is the final result of lane detection**
![Lanedetection](http://i65.tinypic.com/6t20ly.png)

## Dependencies

The project depends on boost::filesystem library for fetching input files from a location. The other dependencies are the OpenCV C++ Library, a C++ 11 compiler and Cmake (Version 2.8 minimum).

Boost Cpp Library can be obtained by following the installation steps provided in this link below :
[Click here to navigate to the official boost cpp libraries page](https://www.boost.org/doc/libs/1_66_0/more/getting_started/unix-variants.html)

OpenCV3 can be installed by following the detailed installation procedure given in this website :
[Click here to navigate to the official opencv installation page](https://www.learnopencv.com/install-opencv3-on-ubuntu/)

CMake build system can be installed by typing this command below in your Ubuntu terminal :
```
sudo apt-get install cmake
```

## Product backlog, iteration backlog and work log

Please click the following link for viewing the product backlog, iteration backlog and work log for this project :

[Click here to access the file on Google sheets](https://docs.google.com/spreadsheets/d/1mCRqtXPC9wf_Tl6CTOj943SKXVybF-TyjP9tHChutUs/edit?usp=sharing)

## Standard install via command-line
```
git clone -b GMock_Extra_Credit https://github.com/arunumd/LaneDetection.git
cd LaneDetection
mkdir build
cd build
cmake .. && make
```
You MUST place the vendor folder inside /Lanedetection before running the *cmake .. && make* command

## Doxygen documentation

If you don't have doxygen already installed on your computer, then please do this install step below :
```
sudo apt-get install doxygen
```

After ensuring doxygen is installed in your computer, you can now create doxygen style documentation using this syntax below:
```
cd <path to repository>
cd doxygen
doxygen doxygenSettings.conf
```

Now you should be able to find the doxygen generated documentation files in ../doxygen/html and ../doxygen/latex folders

## Building for code coverage
```
sudo apt-get install lcov
cmake -D COVERAGE=ON -D CMAKE_BUILD_TYPE=Debug ../
make
make code_coverage
```
This generates a index.html page in the build/coverage sub-directory that can be viewed locally in a web browser.

## Working with Eclipse IDE ##

## Installation

In your Eclipse workspace directory (or create a new one), checkout the repo (and submodules)
```
mkdir -p ~/workspace
cd ~/workspace
git clone --recursive https://github.com/arunumd/LaneDetection
```

In your work directory, use cmake to create an Eclipse project for an [out-of-source build] of LaneDetection

```
cd ~/workspace
mkdir -p LaneDetectionProject
cd LaneDetectionProject
cmake -G "Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug -D CMAKE_ECLIPSE_VERSION=4.7.0 -D CMAKE_CXX_COMPILER_ARG1=-std=c++11 ../LaneDetection/
```

## Import

Open Eclipse, go to File -> Import -> General -> Existing Projects into Workspace -> 
Select "LaneDetectionProject" directory created previously as root directory -> Finish

# Edit

Source files may be edited under the "[Source Directory]" label in the Project Explorer.


## Build

To build the project, in Eclipse, unfold LaneDetectionProject project in Project Explorer,
unfold Build Targets, double click on "all" to build all projects.

## Run

1. In Eclipse, right click on the boilerplate-eclipse in Project Explorer,
select Run As -> Local C/C++ Application

2. Choose the binaries to run (e.g. shell-app, cpp-test for unit testing)


## Debug


1. Set breakpoint in source file (i.e. double click in the left margin on the line you want 
the program to break).

2. In Eclipse, right click on the boilerplate-eclipse in Project Explorer, select Debug As -> 
Local C/C++ Application, choose the binaries to run (e.g. shell-app).

3. If prompt to "Confirm Perspective Switch", select yes.

4. Program will break at the breakpoint you set.

5. Press Step Into (F5), Step Over (F6), Step Return (F7) to step/debug your program.

6. Right click on the variable in editor to add watch expression to watch the variable in 
debugger window.

7. Press Terminate icon to terminate debugging and press C/C++ icon to switch back to C/C++ 
perspetive view (or Windows->Perspective->Open Perspective->C/C++).


## Plugins

- CppChEclipse

    To install and run cppcheck in Eclipse

    1. In Eclipse, go to Window -> Preferences -> C/C++ -> cppcheclipse.
    Set cppcheck binary path to "/usr/bin/cppcheck".

    2. To run CPPCheck on a project, right click on the project name in the Project Explorer 
    and choose cppcheck -> Run cppcheck.


- Google C++ Sytle

    To include and use Google C++ Style formatter in Eclipse

    1. In Eclipse, go to Window -> Preferences -> C/C++ -> Code Style -> Formatter. 
    Import [eclipse-cpp-google-style][reference-id-for-eclipse-cpp-google-style] and apply.

    2. To use Google C++ style formatter, right click on the source code or folder in 
    Project Explorer and choose Source -> Format

[reference-id-for-eclipse-cpp-google-style]: https://raw.githubusercontent.com/google/styleguide/gh-pages/eclipse-cpp-google-style.xml

- Git

    It is possible to manage version control through Eclipse and the git plugin, but it typically requires creating another project. If you're interested in this, try it out yourself and contact me on Canvas.
