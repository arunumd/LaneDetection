/************************************************************************************************
* @file      : Header file for Thresholder class
* @author    : Arun Kumar Devarajulu
* @brief     : The Thresholder class creates an L*a*b color threshold for the lanes in
*              road images
* @date      : October 8, 2018
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

class Thresholder {
 public:
    /***
    *@brief  : Default constructor for Thresholder class
    *@params : wMin is minimum threshold for white color
    *@params : wMax is maximum threshold for white color
    *@params : yMin is minimum threshold for yellow color
    *@params : yMax is maximum threshold for yellow color
    *****/
    Thresholder(cv::Scalar wMin, cv::Scalar wMax, cv::Scalar yMin, \
                cv::Scalar yMax) : whiteMin(wMin), whiteMax(wMax), \
                yellowMin(yMin), yellowMax(yMax) {}

    ~Thresholder() {}   // <Default destructor for Thresholder class

    /***
    *@brief  : The convertToLab() function converts the input BGR image into an
    *          L*a*b color space image
    *@params : The parameter smoothImg is the input image with GaussianBlur
    *****/
    cv::Mat convertToLab(cv::Mat smoothImg);

    /****
    *@brief  : The whiteMaskFunc() creates a white lanes mask based on the
    *          minimum and maximum range of white color threshold
    *******/
    cv::Mat whiteMaskFunc();

    /****
    *@brief  : The yellowMaskFunc() creates a yellow lanes mask based on the
    *          minimum and maximum range of yellow color threshold
    *******/
    cv::Mat yellowMaskFunc();

    /****
    *@brief  : The combinelanes() combines the outputs obtained from
    *          whiteMaskFunc() and yellowMaskFunc() and creates a binary
    *          image with only yellow and white lanes on the road
    *******/
    cv::Mat combineLanes();

 private:
    cv::Mat inputImg;   // < Container used for storing input image
    const cv::Scalar whiteMin;   // < Minimum threshold for white lane
    const cv::Scalar whiteMax;   // < Maximum threshold for white lane
    const cv::Scalar yellowMin;   // < Minimum threshold for yellow lane
    const cv::Scalar yellowMax;   // < Maximum threshold for yellow lane
    cv::Mat whiteMask;   // < Container for white lanes
    cv::Mat yellowMask;   // < Container for yellow lanes
    cv::Mat lanesMask;   // < Container for all lanes combined
    cv::Mat labImage;   // < Container for LAB converted input image
};
