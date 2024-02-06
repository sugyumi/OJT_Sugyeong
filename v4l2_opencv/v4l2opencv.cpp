#include <opencv2/opencv.hpp>
#include <iostream>
#include <linux/videodev2.h>
using namespace cv;
int main(int ac, char** av) {
	std::string gst_cmd = "v4l2src ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! videoconvert ! appsink";
	//v4l2src ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! videoconvert ! appsink
	cv::VideoCapture cap(gst_cmd);
	cap.open(0, CAP_V4L2);
	if (!cap.isOpened())
	{
		std::cout << "Can't open the camera" << std::endl;
		return -1;
	}

	cv::Mat img;
	system("date");
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
	system("date");
	return 0;
}
