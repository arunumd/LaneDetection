/************************************************************************************************
* @file      : Header file for image cleaner  class
* @author    : Arun Kumar Devarajulu
* @brief     : The Cleaner class is used for performing the following operations on input
*              image frames:
*                1.) Undistort the input image frame using camera parameters and distortion
*                    coefficients; and
*                2.) Smoothen the undistorted image using a gaussian filter.
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
*
*
**************************************************************************************************/
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

class Cleaner {
 public:
    /**
    * @brief  : The default constructor for Cleaner class
    * @params : cParam is the camera Parameters
    * @params : dCoeffs is the distortion coefficients
    *
    ****/
    Cleaner(cv::Mat cParam, cv::Mat dCoeffs) : \
        camParams(cParam), distCoeffs(dCoeffs) {}
    ~Cleaner() {}   // <Default destructor for Cleaner class

    /**
    *
    * @brief  : The function imgUndistort is used for undistorting an input image
    * @params : rawImg is the input image from video frames
    *
    ****/
    void imgUndistort(cv::Mat rawImg);

    /***
    *
    * @brief  : the function imgSmoothen applies a gaussian blur on undistorted image
    *
    *****/
    cv::Mat imgSmoothen();

 private:
    cv::Mat camParams;   // < Container for Camera parameters
    cv::Mat distCoeffs;   // < Container for distortion coefficients
    cv::Mat rawImage;   // < Container for input image
    cv::Mat blurImage;   // < Container for denoised image
    cv::Mat undistortedImage;   // < Container for undistorted image
};
