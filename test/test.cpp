/************************************************************************************************
* @file      : Google test based unit test file for Autonomous vehicle lane detection
*              using OpenCV and C++
* @author    : Arun Kumar Devarajulu
* @brief     : The following lines of code tests all the classes and their functions
*              which are located in ../app/ and ../include/ directories. Diles.cpp is
*              excluded from testing because it interfaces with system dependencies
*              which cannot be tested.
* @date      : October 16, 2018
* @copyright : 2018, Arun Kumar Devarajulu
* @license   : MIT License
*
*              Permission is hereby granted, free of charge, to any person obtaining a copy
*              of this software and associated documentation files (the "Software"), to deal
*              in the Software without restriction, including without limitation the rights
*              to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*              copies of the Software, and to permit persons to whom the Software is
*              furnished to do so, subject to the following conditions:
*
*              The above copyright notice and this permission notice shall be included in all
*              copies or substantial portions of the Software.
*
*              THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*              IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*              FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*              AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*              LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*              OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*              SOFTWARE.
*************************************************************************************************/
#include "gtest/gtest.h"
#include "Cleaner.hpp"
#include "Thresholder.hpp"
#include "LanesMarker.hpp"
#include "RegionMaker.hpp"
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"

template <typename T> std::string type_name();

/************************************
*
*  First we test the Cleaner class
*
*************************************/
TEST(CleanerTest, UndistortAndBlurTest) {
    Cleaner CleanerObj((cv::Mat_<double>(3, 3) << 1.15422732e+03, \
                        0.00000000e+00, 6.71627794e+02, 0.00000000e+00, \
                        1.14818221e+03, 3.86046312e+02, 0.00000000e+00, \
                        0.00000000e+00, 1.00000000e+00),  \
                       (cv::Mat_<double>(1, 8) << -2.42565104e-01, \
                        -4.77893070e-02, -1.31388084e-03, \
                        -8.79107779e-05, 2.20573263e-02, 0, 0, 0));
    cv::Mat sampleImg = cv::Mat::ones(100, 100, CV_8UC3);
    CleanerObj.imgUndistort(sampleImg);
    auto undistortOutput = CleanerObj.imgSmoothen();

    EXPECT_EQ(sampleImg.type(), undistortOutput.type());
    std::cout << "Undistort and smoothen outputs are good" << std::endl;
}

/***************************************
*
*  Next we test the Thresholder class
*
****************************************/
TEST(ThresholderTest, LabAndMaskTest) {
    Thresholder ThresholdObj(cv::Scalar(198, 0, 0), \
                             cv::Scalar(255, 255, 255), \
                             cv::Scalar(165, 130, 130), \
                             cv::Scalar(255, 255, 255));;

    cv::Mat sampleImg = cv::Mat::ones(100, 100, CV_8UC3);
    auto LabOutput = ThresholdObj.convertToLab(sampleImg);
    EXPECT_EQ(sampleImg.type(), LabOutput.type());
    std::cout << "L*a*b output is good" << std::endl;

    auto whiteMaskOutput = ThresholdObj.whiteMaskFunc();
    EXPECT_NE(sampleImg.type(), whiteMaskOutput.type());
    std::cout << "White mask output is good" << std::endl;

    auto yellowMaskOutput = ThresholdObj.yellowMaskFunc();
    EXPECT_NE(sampleImg.type(), yellowMaskOutput.type());
    std::cout << "Yellow mask output is good" << std::endl;

    auto combineLanesOutput = ThresholdObj.combineLanes();
    EXPECT_NE(sampleImg.type(), combineLanesOutput.type());
    std::cout << "Combine lanes output is good" << std::endl;
}

/*********************************************
*
*  Later we test the LanesMarker class
*
**********************************************/
TEST(LanesMarkerTest, HoughLinesTest) {
    std::vector<float> dummyRho(30), dummyTheta(30);
    std::iota(dummyRho.begin(), dummyRho.end(), -13.2);
    std::iota(dummyTheta.begin(), dummyTheta.end(), -7.4);
    std::vector<cv::Vec2f> linesParams;
    for (int i = 0; i < 30; i++) {
        linesParams.emplace_back(dummyRho[i], dummyTheta[i]);
    }

    LanesMarker LanesObj;
    LanesObj.lanesSegregator(linesParams);
    auto dummyPair = std::make_pair(-55.01, -77.89);
    auto leftLanes = LanesObj.leftLanesAverage();
    auto rightLanes = LanesObj.rightLanesAverage();

    EXPECT_NE(typeid(dummyPair).name(), typeid(leftLanes).name());
    EXPECT_NE(typeid(dummyPair).name(), typeid(rightLanes).name());
}

/************************************************
*
*  At the end we test the RegionMaker class
*
*************************************************/
TEST(RegionMakerTest, PolygonTest) {
    cv::Mat sampleImg = cv::Mat::ones(100, 100, CV_8UC3);
    RegionMaker polygonObj;
    auto polyVecType = polygonObj.getPolygonVertices(sampleImg);
    EXPECT_EQ(typeid(std::vector<cv::Point>).name(), \
              typeid(polyVecType).name());
}
