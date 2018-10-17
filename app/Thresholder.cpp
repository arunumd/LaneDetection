/************************************************************************************************
* @file      : Implementation file for Thresholder class
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
#include "Thresholder.hpp"

/***
*@brief  : The convertToLab() converts an input BGR image to an output L*a*b
*          image using the OpenCV function cv::cvtColor.
*@params : The parameter smoothImg is the GaussianBlurred image
*@return : The return value is a cv::Mat containing the L*a*b image
*****/
cv::Mat Thresholder::convertToLab(cv::Mat smoothImg) {
    inputImg = smoothImg;
    cv::cvtColor(inputImg, labImage, cv::COLOR_BGR2Lab);
    return labImage;
}

/***
*@brief  : The whiteMaskFunc() creates a binary thresholded image based on the
*          white color threshold range for the white lanes. For accomplishing this
*          task we use the OpenCV cv::inrange function
*@return : The output returned by this function is a binary image with ones in white
*          lanes region and zeros everywhere else
*****/
cv::Mat Thresholder::whiteMaskFunc() {
    whiteMask = cv::Mat::zeros(lanesMask.size(), CV_8U);
    cv::inRange(labImage, whiteMin, whiteMax, whiteMask);
    return whiteMask;
}

/***
*@brief  : The yellowMaskFunc() creates a binary thresholded image based on the
*          yellow color threshold range for the yellow lanes. For accomplishing this
*          task we use the OpenCV cv::inrange function
*@return : The output returned by this function is a binary image with ones in yellow
*          lanes region and zeros everywhere else
*****/
cv::Mat Thresholder::yellowMaskFunc() {
    yellowMask = cv::Mat::zeros(lanesMask.size(), CV_8U);
    cv::inRange(labImage, yellowMin, yellowMax, yellowMask);
    return yellowMask;
}

/***
*@brief  : The combineLanes() is used for combining the outputs obtained from the
*          whiteMaskFunc() and the yellowMaskFunc(). We use the cv::bitwise_or function
*          for combining the lanes masks based on boolean OR logic.
*@return : The output returned by this function is a cv::lanesMask matrix containing
*          both the white and yellow lanes
*****/
cv::Mat Thresholder::combineLanes() {
    lanesMask = cv::Mat::zeros(lanesMask.size(), CV_8U);
    cv::bitwise_or(whiteMask, yellowMask, lanesMask);
    return lanesMask;
}

