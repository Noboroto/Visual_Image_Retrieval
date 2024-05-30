#include <iostream>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include "histogram.hpp"

// ref at https://docs.opencv.org/3.4/d0/d2e/classcv_1_1CommandLineParser.html
using cv::CommandLineParser;
using cv::imshow;
using cv::Mat;
using cv::namedWindow;
using cv::VideoCapture;
using cv::waitKey;
using std::cout;
using std::string;

int main(int argc, char **argv)
{
	// The logging level can be set with the following function
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

	const string keys =
		"{help h usage ? | | print this message }"
		"{path || path to file }"
		"{camid |0| The camera ID on the system }";

	CommandLineParser parser(argc, argv, keys);
	string path = parser.get<string>("path");
	int camid = parser.get<int>("camid");

	if (parser.has("help"))
	{
		parser.printMessage();
		system("pause");
		return 0;
	}

	if (!parser.check())
	{
		parser.printErrors();
		system("pause");
		return 0;
	}
	string title = "";
	VideoCapture cap;
	if (path.length() > 0)
	{
		title = "Video: " + path;
		cout << "Video path: " << path << std::endl;
		cap.open(path);
	}
	else
	{
		title = "Video: Camera " + std::to_string(camid);
		cout << "Camera ID: " << camid << std::endl;
		cap.open(camid);
	}
	if (!cap.isOpened())
	{
		if (path.length() > 0)
		{
			cout << "Could not open the video file" << std::endl;
		}
		else
		{
			cout << "Could not open the camera" << std::endl;
		}
		return -1;
	}

	namedWindow(title, 1);
	for (;;)
	{
		Mat frame;
		while (frame.empty())
		{
			cap >> frame; // get a new frame from camera
		}
		imshow(title, frame);
		showHistogram(frame);
		if (waitKey(30) >= 0)
			break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}