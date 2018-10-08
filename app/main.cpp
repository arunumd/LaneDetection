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

		videofile >> frame;

		if (frame.empty ())
			break;

		cv::imshow("Video frames", frame);

		char c = (char) cv::waitKey (0);
		if (c == 27)
			break;
	}

	videofile.release ();

	cv::destroyAllWindows ();

	return 0;
}