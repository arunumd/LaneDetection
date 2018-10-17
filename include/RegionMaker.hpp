/************************************************************************************************
* @file      : Header file for RegionMaker class
* @author    : Arun Kumar Devarajulu
* @brief     : The RegionMaker class is used for creating a polygonal region based on
*              binary points fetched from HoughLine regions
* @date      : October 15, 2018
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
#include <vector>
#include <string>
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

class RegionMaker {
 public:
    RegionMaker() {}  // <Default constructor
    ~RegionMaker() {}  // <Default destructor

    /***
    *@brief  : The getPolygonVertices() function is used for fetching a vector of
    *          polygon corners from an input matrix of points
    *@params : The parameter cv::Mat binaryPoints is an input image matrix with
    *          ones in HoughLine regions and zeros everywhere else
    *@return : The output from this function is a vector of polygon points
    *****/
    std::vector<cv::Point> getPolygonVertices(cv::Mat binaryPoints);

 private:
    cv::Point polyVertex1;  // <Variable for storing polygon bottom right corner
    cv::Point polyVertex2;  // <Variable for storing polygon top left corner
    cv::Point polyVertex3;  // <Variable for storing polygon top right corner
    cv::Point polyVertex4;  // <Variable for storing polygon bottom left corner
    double low = 0;  // <Minimum assumption for optimization problem
    double high = 5000;  // <Maximum assumption for optimization problem
    // Container for storing polygon vertices
    std::vector<cv::Point> polygonVertices;
};

