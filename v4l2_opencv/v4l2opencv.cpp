#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <linux/videodev2.h>
using namespace cv;
int main(int ac, char** av) {
	/* "v4l2src ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! videoconvert ! appsink";
	std::string device = "/dev/video0"; */

//#define OPTION
	
#ifdef OPTION
	int api = cv::CAP_ANY;
#else
	int api = cv::CAP_V4L2;
#endif

	int deviceID = 0;
	cv::VideoCapture cap;
	cap.open(deviceID, api);
	if (!cap.isOpened())
	{
		std::cout << "Can't open the camera" << std::endl;
		return -1;
	}
	cv::Mat img;

#ifdef OPTION
	while (cap.isOpened())
	{
		try
		{
			cap >> img;
			
			if (img.empty())
			{
				std::cout << "empty image" << std::endl;
				return 0;
			}
			
			cv::imshow("camera img", img);

			if (cv::waitKey(25) == 27)
				break;
		}
		catch( cv::Exception& e )
		{
			const char* err_msg = e.what();
			std::cout << "exception caught: " << err_msg << std::endl;
		}
	}
#else
	for (;;)
	{
		cap.read(img);
		if (img.empty()) {
			std::cerr << "ERROR! blank img\n";
			break;
		}
		imshow("Live", img);
		if (waitKey(5) >= 0) {
			break;
		}
	}
#endif
	return 0;
}
