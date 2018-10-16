/************************************************************************************************
* @file      : main file for Autonomous vehicle lane detection using OpenCV and C++
* @author    : Arun Kumar Devarajulu
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
#include <tuple>
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
#include "Files.hpp"
#include "Cleaner.hpp"
#include "Thresholder.hpp"
#include "LanesMarker.hpp"
#include "RegionMaker.hpp"

namespace FS = boost::filesystem;    //! Short form for boost filesystem

int main(int argc, char *argv[]) {
	cv::Point p;
	//  Hardcoding certain parameters like screen area to search for, etc.
	std::vector<cv::Point> roiPoints;   // <First fillConvexPoly points
	roiPoints.push_back(cv::Point(527, 491));
	roiPoints.push_back(cv::Point(812, 491));
	roiPoints.push_back(cv::Point(1163, 704));
	roiPoints.push_back(cv::Point(281, 704));


	//  Dummy variable for temporary points storage in HoughLines
	std::pair <cv::Point2d, cv::Point2d> vertices;

	// Variable stores rho, theta from cv::HoughLines()
	std::vector<cv::Vec2f> lines;

	//  Initialize the Files class as an object
	Files location;

	std::string fileAddress;    //  <String variable for holding file name

	/****************************************************************
	*
	*  @Brief: The following lines of code perform sanity checks
	*          for valid filename containing valid input file
	*
	****************************************************************/

	if (argc < 2) {
		std::cout << "Please enter directory location in command prompt\n";
		std::getline(std::cin, fileAddress);
		fileAddress = location.filePicker(fileAddress);
	} else if (argc == 2) {
		fileAddress = argv[1];
		fileAddress = location.filePicker(fileAddress);
	} else {
		std::cout << "The file path cannot contain empty spaces\n"
		          "please enter valid path without spaces.";
		std::getline(std::cin, fileAddress);
		fileAddress = location.filePicker(fileAddress);
	}

	/****************************************************************************
	*
	*  @Brief: The following lines of code iterate sequentially
	*          between image frames in input video file and
	*          perform the following lane detection pipeline :
	*           1.) Undistort the input image frames;
	*           2.) Smoothen the undistorted image frames;
	*           3.) Threshold the smoothened image frames with LAB color space;
	*           4.) Detect edges corresponding to road lanes;
	*           5.) Draw Hough Lines on the basis of the detected edges; and
	*           6.) Mark lanes based on the Hough Lines.
	*
	*****************************************************************************/
	cv::VideoCapture videofile(fileAddress);

	if (!videofile.isOpened()) {
		std::cout << "Error opening input video file" << std::endl;
		return -1;
	}

	while (1) {
		lines.clear();   // Emptying the container from previous iteration
		cv::Mat frame;

		videofile >> frame;  //  <Grab the image frame

		if (frame.empty())
			break;

		Cleaner imgClean((cv::Mat_<double>(3, 3) << 1.15422732e+03, \
		                  0.00000000e+00, 6.71627794e+02, 0.00000000e+00, \
		                  1.14818221e+03, 3.86046312e+02, 0.00000000e+00, \
		                  0.00000000e+00, 1.00000000e+00),  \
		                 (cv::Mat_<double>(1, 8) << -2.42565104e-01, \
		                  -4.77893070e-02, -1.31388084e-03, \
		                  -8.79107779e-05, 2.20573263e-02, 0, 0, 0));

		imgClean.imgUndistort(frame);

		cv::Mat blurImg;

		blurImg = imgClean.imgSmoothen();

		Thresholder lanethresh(cv::Scalar(198, 0, 0), \
		                       cv::Scalar(255, 255, 255), \
		                       cv::Scalar(165, 130, 130), \
		                       cv::Scalar(255, 255, 255));

		cv::Mat labOutput;

		labOutput = lanethresh.convertToLab(blurImg);

		cv::Mat whiteOutput;

		whiteOutput = lanethresh.whiteMaskFunc();

		cv::Mat yellowOutput;

		yellowOutput = lanethresh.yellowMaskFunc();

		cv::Mat lanesMask;

		lanesMask = lanethresh.combineLanes();

		cv::imshow("Lanes Mask", lanesMask);

		cv::Mat firstPolygonArea(lanesMask.rows, lanesMask.cols, CV_8U, cv::Scalar(0));

		cv::Mat interestLanes = cv::Mat::zeros(lanesMask.size(), CV_8U);

		cv::fillConvexPoly(firstPolygonArea, roiPoints, cv::Scalar(1));

		lanesMask.copyTo(interestLanes, firstPolygonArea);

		cv::Mat edges = cv::Mat::zeros(lanesMask.size(), CV_8U);

		cv::Canny(interestLanes, edges, 15, 45, 3);

		imshow("Canny Output", edges);

		cv::HoughLines(edges, lines, 1, CV_PI / 180, 10, 0, 0);

		LanesMarker lanesConsole;

		lanesConsole.lanesSegregator(lines);

		auto left = lanesConsole.leftLanesAverage();

		auto right = lanesConsole.rightLanesAverage();

		cv::Mat black_img = cv::Mat::zeros(labOutput.size(), labOutput.type());

		cv::line(black_img, left.first, left.second, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);

		cv::line(black_img, right.first, right.second, cv::Scalar(0, 0, 255), 3, cv::LINE_AA);

		cv::Mat polygonLayer = cv::Mat::zeros(labOutput.size(), labOutput.type());

		cv::Mat linesCanny = polygonLayer.clone();

		black_img.copyTo(polygonLayer, firstPolygonArea);

		cv::Canny(polygonLayer, linesCanny, 70, 210, 3);

		cv::Mat binaryRegions;

		cv::findNonZero(linesCanny, binaryRegions);

		RegionMaker polyMaker;

		auto polyRegionVertices = polyMaker.getPolygonVertices(binaryRegions);

		cv::Mat dummy = cv::Mat::zeros(labOutput.size(), labOutput.type());

		cv::fillConvexPoly(frame, polyRegionVertices, cv::Scalar(255, 0, 0), CV_AA, 0);

		cv::imshow("Lines canny", frame);

		cv::imshow("Hough Lines", black_img);

		// /home/arun/Downloads/challenge_video.mp4


		char c = static_cast<char> (cv::waitKey(0));
		if (c == 27)
			break;
	}

	videofile.release();

	cv::destroyAllWindows();

	return 0;
}
