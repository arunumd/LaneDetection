/************************************************************************************************
* @file      : Implementation for image cleaner  class
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
#include "Cleaner.hpp"

/***
* @brief  : The imgUndistort function takes in the raw image and undistorts the image
*           using cv::undistort() function. The parameters camParams, distCoeffs are
*           initialised by the Class constructor
* @params : The parameter rawImg is the input image frame
****/
void Cleaner::imgUndistort(cv::Mat rawImg) {
    rawImage = rawImg;
    cv::undistort(rawImage, undistortedImage, camParams, distCoeffs);
}

/***
* @brief  : The imgSmoothen function creates a new image which is a gaussin blurred version
*           of the undistorted image. For this we first create a matrix of zeros of the size
*           and type of undistorted image. Later we pass this matrix to the cv::GaussianBlur()
*           function. A kernel size of (5, 5) is used here. At the end of the GaussianBlur
*           operation, the blurImage will be a an undistorted image with a Gaussian Blur.
* @return : The blured image
****/
cv::Mat Cleaner::imgSmoothen() {
    blurImage = cv::Mat::zeros(undistortedImage.size(), \
                               undistortedImage.type());
    cv::GaussianBlur(undistortedImage, blurImage, cv::Size(5, 5), 0, 0);
    return blurImage;
}
