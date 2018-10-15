/************************************************************************************************
* @file      : Header file for LanesMarker class
* @author    : Arun Kumar Devarajulu
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
	typedef std::pair<cv::Point2d, cv::Point2d> pointsPair;
	typedef std::vector<pointsPair> laneType;
	typedef std::vector<cv::Vec2f> hType;
public:
	LanesMarker () {}  // Default constructor
	~LanesMarker () {}  // Default destructor
	void lanesSegregator(hType hLines);

	laneType getLeftLane();

	laneType getRightLane();

	pointsPair leftLanesAverage();

	pointsPair rightLanesAverage();

private:
	float rho = 0;
	float theta = 0;
	cv::Point2d pt1, pt2;
	double slope = 0;
	double a = 0, b = 0;
	double x0, y0;
	std::pair <cv::Point2d, cv::Point2d> vertices;
	laneType lLane, rLane;
	double countLeft = 0;
	double countRight = 0;
	double pt1xLeft = 0;
	double pt1xRight = 0;
	double pt2xLeft = 0;
	double pt2xRight = 0;
	double pt1yLeft = 0;
	double pt1yRight = 0;
	double pt2yLeft = 0;
	double pt2yRight = 0;
	cv::Point2d avgPoint1Left, avgPoint2Left;
	cv::Point2d avgPoint1Right, avgPoint2Right;
};
