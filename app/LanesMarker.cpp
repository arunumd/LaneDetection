/************************************************************************************************
* @file      : Implementation file for LanesMarker class
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
#include "LanesMarker.hpp"

typedef std::pair<cv::Point2d, cv::Point2d> pointsPair;  //<Short form for a pair of cv::Point2d
typedef std::vector<pointsPair> laneType;  // <Short form for a vector of type pointsPair
typedef std::vector<cv::Vec2f> hType;  // <Short form for a vector of type cv::Vec2f

/***
*@brief  : The lanesSegregator() function takes in the cv::HoughLines function
*          output and classifies the HoughLines as belonging to left lane and
*          the right lane based on the slopes.
*@params : The input parameter hLines is of type hType as defined in the typedef
*          statement above
*****/

void LanesMarker::lanesSegregator(hType hLines) {
		for (auto& item : hLines) {
		rho = item[0], theta = item[1];
		a = std::cos(theta), b = std::sin(theta);
		x0 = a * rho, y0 = b * rho;
		pt1.x = cvRound(x0 + 1500 * (-b));
		pt1.y = cvRound(y0 + 1500 * (a));
		pt2.x = cvRound(x0 - 1500 * (-b));
		pt2.y = cvRound(y0 - 1500 * (a));

		slope = ((pt2.y - pt1.y) / (pt2.x - pt1.x));

		if ((slope >= -0.75) && (slope <= -0.65)) { //Creation of left lanes set
			vertices = std::make_pair(pt1, pt2);
			//std::cout << "The slope Left is : " << slope << std::endl;
			lLane.push_back(vertices);
		} else if ((slope >= 0.80) && (slope <= 1.33)) { //Creation of right lanes set
			vertices = std::make_pair(pt1, pt2);
			//std::cout << "The slope Right is : " << slope << std::endl;
			rLane.push_back(vertices);
		}
	}
}

/***
*@brief  : The leftLanesAverage() function is used for averaging all the left lanes
*          to get only one unique left lane
*@return : The output returned by this function is a pair of top and bottom points
*          belonging to the left lane
******/

pointsPair LanesMarker::leftLanesAverage() {
	for (auto& item : lLane) {
		cv::Point2d V1;
		V1 = item.first;
		pt1xLeft = pt1xLeft + V1.x;
		pt1yLeft = pt1yLeft + V1.y;

		cv::Point2d V2;
		V2 = item.second;
		pt2xLeft = pt2xLeft + V2.x;
		pt2yLeft = pt2yLeft + V2.y;
	}

	countLeft = lLane.size();

	avgPoint1Left.x = pt1xLeft / countLeft;
	avgPoint1Left.y = pt1yLeft / countLeft;
	avgPoint2Left.x = pt2xLeft / countLeft;
	avgPoint2Left.y = pt2yLeft / countLeft;

	return std::make_pair(avgPoint1Left, avgPoint2Left);
}

/***
*@brief  : The rightLanesAverage() function is used for averaging all the right lanes
*          to get only one unique right lane
*@return : The output returned by this function is a pair of top and bottom points
*          belonging to the right lane
******/
pointsPair LanesMarker::rightLanesAverage() {
	for (auto& item : rLane) {
		cv::Point2d V1;
		V1 = item.first;
		pt1xRight = pt1xRight + V1.x;
		pt1yRight = pt1yRight + V1.y;

		cv::Point2d V2;
		V2 = item.second;
		pt2xRight = pt2xRight + V2.x;
		pt2yRight = pt2yRight + V2.y;
	}

	countRight = rLane.size();

	avgPoint1Right.x = pt1xRight / countRight;
	avgPoint1Right.y = pt1yRight / countRight;
	avgPoint2Right.x = pt2xRight / countRight;
	avgPoint2Right.y = pt2yRight / countRight;

	return std::make_pair(avgPoint1Right, avgPoint2Right);
}