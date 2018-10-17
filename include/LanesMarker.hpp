/************************************************************************************************
* @file      : Header file for LanesMarker class
* @author    : Arun Kumar Devarajulu
* @brief     : The LanesMarker class is used for segregating all the HoughLines into left and right
*              categories and then obtain unique left and right lanes by averaging according to
*              positive and negative slopes
* @date      : October 14, 2018
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
#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <tuple>
#include <utility>
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

class LanesMarker {
    // Short form for a pair of cv::Point2d
    typedef std::pair<cv::Point2d, cv::Point2d> pointsPair;
    // Short form for a vector of type pointsPair
    typedef std::vector<pointsPair> laneType;
    // Short form for a vector of type cv::Vec2f
    typedef std::vector<cv::Vec2f> hType;

 public:
    LanesMarker() {}  // Default constructor
    ~LanesMarker() {}  // Default destructor

    /***
    *@brief  : The lanesSegregator() is used for segregating the left and right lanes based on
    *          positive and negative slopes of HoughLines
    *@params : The parameter hLines is the output obtained from cv::HoughLines function. It is
    *          nothing but an array of pairs containing rho and theta
    *****/
    void lanesSegregator(hType hLines);

    /***
    *@brief  : The leftLanesAverage() is used for averaging the left lanes based on negative slopes
    ******/
    pointsPair leftLanesAverage();

    /***
    *@brief  : The rightLanesAverage() is used for averaging the left lanes based on positive slopes
    ******/
    pointsPair rightLanesAverage();

 private:
    // Variable for temporary storage of HoughLines rho
    float rho = 0;
    // Variable for temporary storage of HoughLines theta
    float theta = 0;
    // Dummy variables for extrapolating vertices based on rho and theta
    cv::Point2d pt1, pt2;
    // Variable for calculating slope
    double slope = 0;
    // variables for temporary storage of intercepts
    double a = 0, b = 0;
    // Variables for temporary storage of intercepts
    double x0, y0;
    // Variables for pushing points to vector
    std::pair <cv::Point2d, cv::Point2d> vertices;
    // Containers for storing all the left and right lanes
    laneType lLane, rLane;
    //  Variable for count of left lanes
    double countLeft = 0;
    //  Variable for count of right lanes
    double countRight = 0;
    // Variable for summing left lanes top x co-ordinates
    double pt1xLeft = 0;
    // Variable for summing right lanes top x co-ordinates
    double pt1xRight = 0;
    // Variable for summing left lanes bottom x co-ordinates
    double pt2xLeft = 0;
    // Variable for summing right lanes bottom x co-ordinates
    double pt2xRight = 0;
    // Variable for summing left lanes top y co-ordinates
    double pt1yLeft = 0;
    // Variable for summing right lanes top y co-ordinates
    double pt1yRight = 0;
    // Variable for summing left lanes bottom y co-ordinates
    double pt2yLeft = 0;
    // Variable for summing right lanes bottom y co-ordinates
    double pt2yRight = 0;
    // Variables for storing final best points for left lanes
    cv::Point2d avgPoint1Left, avgPoint2Left;
    // Variables for storing final best points for right lanes
    cv::Point2d avgPoint1Right, avgPoint2Right;
};

