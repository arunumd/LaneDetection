/************************************************************************************************
* @file      : main file for Autonomous vehicle lane detection using OpenCV and C++
* @author    : Arun Kumar Devarajulu
* @brief     : The following lines of code iterate sequentially
*              between image frames in input video file and
*              perform the following lane detection pipeline :
*                 1.) Undistort the input image frames;
*                 2.) Smoothen the undistorted image frames;
*                 3.) Threshold the smoothened image frames with LAB color space;
*                 4.) Detect edges corresponding to road lanes;
*                 5.) Draw Hough Lines on the basis of the detected edges; and
*                 6.) Mark lanes based on the Hough Lines.
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
#include <cstdlib>
#include <cmath>
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
    std::vector<cv::Point> historicLane;
    cv::Point dummy;
    dummy.x = 0;
    dummy.y = 0;
    int counter = 1;
    for (int i = 0; i < 4; i++) {
        historicLane.push_back(dummy);
    }
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

    cv::VideoCapture videofile(fileAddress);

    if (!videofile.isOpened()) {
        std::cout << "Error opening input video file" << std::endl;
        return -1;
    }

    int videoWidth = videofile.get(CV_CAP_PROP_FRAME_WIDTH);
    int videoHeight = videofile.get(CV_CAP_PROP_FRAME_HEIGHT);

    // Here we create a video writing object to write our output
    cv::VideoWriter video("../results/LanesDetection.avi",
                          CV_FOURCC('M', 'J', 'P', 'G'), 10,
                          cv::Size(videoWidth, videoHeight));

    while (1) {
        lines.clear();   // Emptying the container from previous iteration
        cv::Mat frame;

        videofile >> frame;  //  <Grab the image frame

        if (frame.empty())
            break;
        /*****************************************************************
        *
        *  To begin with, we grab the image frames and do pre-processing
        *
        ******************************************************************/

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

        /***************************************************************
        *
        *    After pre-processing we mask the white and yellow lanes
        *
        ****************************************************************/

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

        /****************************************************************
        *
        *  After masking the lanes we get rid of the unnecessary details
        *  like horizon, trees, and other details on the sides of the
        *  roads which can likely interfere with proper detection of lanes
        *
        *****************************************************************/

        cv::Mat firstPolygonArea(lanesMask.rows, lanesMask.cols, \
                                 CV_8U, cv::Scalar(0));
        cv::Mat interestLanes = cv::Mat::zeros(lanesMask.size(), CV_8U);
        cv::fillConvexPoly(firstPolygonArea, roiPoints, cv::Scalar(1));
        lanesMask.copyTo(interestLanes, firstPolygonArea);

        /*****************************************************************
        *
        *   Later we employ a gradient based edge detector to detect
        *   sharp edges which will be our lanes
        *
        ******************************************************************/

        cv::Mat edges = cv::Mat::zeros(lanesMask.size(), CV_8U);
        cv::Canny(interestLanes, edges, 15, 45, 3);
        imshow("Canny Output", edges);

        /******************************************************************
        *
        *  Later we strengthen the detected edges by drawinng Hough Lines
        *  on top of their loci
        *
        *******************************************************************/

        cv::HoughLines(edges, lines, 1, CV_PI / 180, 10, 0, 0);
        LanesMarker lanesConsole;
        lanesConsole.lanesSegregator(lines);
        auto left = lanesConsole.leftLanesAverage();
        auto right = lanesConsole.rightLanesAverage();
        cv::Mat black_img = cv::Mat::zeros(labOutput.size(), \
                                           labOutput.type());
        cv::line(black_img, left.first, left.second, cv::Scalar(0, 0, 255), \
                 3, cv::LINE_AA);
        cv::line(black_img, right.first, right.second, cv::Scalar(0, 0, 255), \
                 3, cv::LINE_AA);

        /*********************************************************************
        *
        *  Later we draw polygonal region on the road which denotes a region
        *  within the bounds of two lanes in front of the vehicle
        *
        *********************************************************************/

        cv::Mat polygonLayer = cv::Mat::zeros(labOutput.size(), \
                                              labOutput.type());
        cv::Mat linesCanny = polygonLayer.clone();
        black_img.copyTo(polygonLayer, firstPolygonArea);
        cv::Canny(polygonLayer, linesCanny, 70, 210, 3);
        cv::Mat binaryRegions;
        cv::findNonZero(linesCanny, binaryRegions);

        RegionMaker polyMaker;
        auto polyRegionVertices = polyMaker.getPolygonVertices(binaryRegions);
        cv::Mat dummy = cv::Mat::zeros(labOutput.size(), labOutput.type());
        for (auto& vertex : polyRegionVertices) {
            if (vertex.x == 0 || vertex.y == 0) {
                polyRegionVertices = historicLane;
                break;
            } else {}
        }
        if (counter > 1 && ((std::abs(polyRegionVertices.at(2).x - \
                                      historicLane.at(2).x) > 10) ||
                            std::abs(polyRegionVertices.at(3).x - \
                                     historicLane.at(3).x) > 10)) {
            polyRegionVertices = historicLane;
        }

        historicLane = polyRegionVertices;

        /*********************************************************************
        *
        *  Now we extrapolate our polygon to fill a desired area on screen
        *
        *********************************************************************/

        auto newSlopeLeft = static_cast<float>(polyRegionVertices.at(0).y - \
                                               polyRegionVertices.at(3).y) /
                            static_cast<float>(polyRegionVertices.at(0).x - \
                                               polyRegionVertices.at(3).x);

        auto newSlopeRight = static_cast<float>(polyRegionVertices.at(1).y - \
                                                polyRegionVertices.at(2).y) /
                             static_cast<float>(polyRegionVertices.at(1).x - \
                                                polyRegionVertices.at(2).x);

        auto leftIntercept = static_cast<float>(polyRegionVertices.at(0).y) - \
                             static_cast<float>(newSlopeLeft) * \
                             static_cast<float>(polyRegionVertices.at(0).x);

        auto rightIntercept = static_cast<float>(polyRegionVertices.at(1).y) - \
                              (static_cast<float>(newSlopeRight) * \
                               static_cast<float>(polyRegionVertices.at(1).x));

        polyRegionVertices.at(0).x = static_cast<double>((550 - \
                                     leftIntercept) / newSlopeLeft);
        polyRegionVertices.at(0).y = 550.0;
        polyRegionVertices.at(1).x = static_cast<double>((550 - \
                                     rightIntercept) / newSlopeRight);
        polyRegionVertices.at(1).y = 550.0;

        cv::fillConvexPoly(frame, polyRegionVertices, \
                           cv::Scalar(0, 255, 0), CV_AA, 0);

        double deviationLeft = std::abs(newSlopeLeft - 1);

        double deviationRight = std::abs(newSlopeRight - 1);

        /*********************************************************************
        *
        *                   At the end we make turn predictions
        *
        *********************************************************************/

        if (deviationRight > deviationLeft) {
            cv::putText(frame, "Left turn ahead", cv::Point(30, 30),
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, \
                        cv::Scalar(200, 200, 250), 1, CV_AA);
        } else if (deviationRight < deviationLeft) {
            cv::putText(frame, "Right turn ahead", cv::Point(30, 30),
                        cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, \
                        cv::Scalar(200, 200, 250), 1, CV_AA);
        } else {}

        video.write(frame);

        cv::imshow("Lines canny", frame);

        counter++;

        char c = static_cast<char> (cv::waitKey(20));
        if (c == 27)
            break;
    }
    video.release();
    videofile.release();

    cv::destroyAllWindows();

    return 0;
}
