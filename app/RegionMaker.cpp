/************************************************************************************************
* @file      : Implementation file for RegionMaker class
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
#include "RegionMaker.hpp"

std::vector<cv::Point> RegionMaker::getPolygonVertices(cv::Mat binaryPoints) {
    for (size_t i = 0; i < binaryPoints.total(); i++) {
        if (binaryPoints.at<cv::Point>(i).x > low \
                && binaryPoints.at<cv::Point>(i).y == 704) {
            polyVertex4.x = binaryPoints.at<cv::Point>(i).x;
            polyVertex4.y = binaryPoints.at<cv::Point>(i).y;
            low = polyVertex4.x;
        } else if (binaryPoints.at<cv::Point>(i).x < high \
                   && binaryPoints.at<cv::Point>(i).y == 704) {
            polyVertex1.x = binaryPoints.at<cv::Point>(i).x;
            polyVertex1.y = binaryPoints.at<cv::Point>(i).y;
            high = polyVertex1.x;
        } else if (binaryPoints.at<cv::Point>(i).x < high \
                   && binaryPoints.at<cv::Point>(i).y == 650) {
            polyVertex2.x = binaryPoints.at<cv::Point>(i).x;
            polyVertex2.y = binaryPoints.at<cv::Point>(i).y;
            high = polyVertex2.x;
        } else if (binaryPoints.at<cv::Point>(i).x > low \
                   && binaryPoints.at<cv::Point>(i).y == 650) {
            polyVertex3.x = binaryPoints.at<cv::Point>(i).x;
            polyVertex3.y = binaryPoints.at<cv::Point>(i).y;
            low = polyVertex3.x;
        } else {}
    }
    polygonVertices.push_back(polyVertex2);
    polygonVertices.push_back(polyVertex3);
    polygonVertices.push_back(polyVertex4);
    polygonVertices.push_back(polyVertex1);

    return polygonVertices;
}
