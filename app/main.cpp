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
#include <tuple>


namespace FS = boost::filesystem; //! Short form for boost filesystem

int main(int argc, char *argv[])
{
	//@Brief: We create some short forms for long type names

	// Short form for file name pairs (for example, <200.jpg, 200>)
	typedef std::pair<FS::path, int> file_entry;
	// Short form for vector of tuples
	typedef std::vector<file_entry> vec;
	// Short form for iterator of type, boost::filesystem::directory_iterator
	//typedef FS::directory_iterator dirIter;
	typedef std::vector <cv::DMatch> matchType;

	typedef std::vector <cv::KeyPoint> keyPointType;

	typedef std::vector <cv::Point2f> goodMatchType;

	//Initialize the Files class as an object
	Files location;

	std::string fileAddress; //< String variable for holding file name

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
	}
	else if (argc == 2) {
		fileAddress = argv [1];
		fileAddress = location.filePicker(fileAddress);
	}
	else {
		std::cout << "The file path cannot contain empty spaces\n\
		please enter valid path without spaces.";
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

	if (!videofile.isOpened ()) {
		std::cout << "Error opening input video file" << std::endl;
		return -1;
	}

	while (1) {

		cv::Mat frame;

		videofile >> frame; //< Grab the image frame

		if (frame.empty ())
			break;

		Cleaner imgClean ((cv::Mat_<double>(3, 3) << 1.15422732e+03, \
		                   0.00000000e+00, 6.71627794e+02, 0.00000000e+00, \
		                   1.14818221e+03, 3.86046312e+02, 0.00000000e+00, \
		                   0.00000000e+00, 1.00000000e+00),  (cv::Mat_<double>(1, 8) << \
		                           -2.42565104e-01, -4.77893070e-02, -1.31388084e-03, \
		                           -8.79107779e-05, 2.20573263e-02, 0, 0, 0));

		imgClean.imgUndistort (frame);

		cv::Mat blurImg;

		blurImg = imgClean.imgSmoothen ();

		Thresholder lanethresh (cv::Scalar(77*255/100, -7+128, -6+128), cv::Scalar(100*255/100, 1+128, 7+128), \
		                        cv::Scalar(80*255/100, -24+128, 50+128), cv::Scalar(100*255/100, 36+128, 128+128));

		cv::Mat labOutput;

		labOutput = lanethresh.convertToLab (blurImg);

		cv::Mat whiteOutput;

		whiteOutput = lanethresh.whiteMaskFunc ();

		cv::Mat yellowOutput;

		yellowOutput = lanethresh.yellowMaskFunc ();

		cv::Mat lanesMask;

		lanesMask = lanethresh.combineLanes ();

		cv::imshow("White Lanes", whiteOutput);

		cv::imshow("Yellow Lanes", yellowOutput);

		cv::imshow("Lanes Mask", lanesMask);

		char c = (char) cv::waitKey (0);
		if (c == 27)
			break;
	}

	videofile.release ();

	cv::destroyAllWindows ();

	return 0;
}