#include <iostream>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
// ref at https://docs.opencv.org/3.4/d0/d2e/classcv_1_1CommandLineParser.html
using cv::CommandLineParser;
using cv::drawKeypoints;
using cv::FeatureDetector;
using cv::imread;
using cv::imshow;
using cv::KeyPoint;
using cv::Mat;
using cv::ORB;
using cv::Ptr;
using cv::Scalar;
using cv::SIFT;
using cv::waitKey;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char **argv)
{
	// The logging level can be set with the following function
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

	const string keys =
		"{help h usage ? | | print this message }"
		"{path || path to file }"
		"{showOutput | | show output image}";

	CommandLineParser parser(argc, argv, keys);

	string path = parser.get<string>("path");
	bool showOutput = parser.has("showOutput");

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

	const Mat input = imread(path);

	Ptr<SIFT> siftPtr = SIFT::create();
	vector<KeyPoint> keypoints_sift;
	siftPtr->detect(input, keypoints_sift);

	// Add results to image and save.
	Mat output_sift;
	drawKeypoints(input, keypoints_sift, output_sift, Scalar(0, 0, 255));
	imwrite(path + "_sift.jpg", output_sift);

	vector<KeyPoint> keypoints_orb;
	Mat output_orb;
	Ptr<FeatureDetector> detector = ORB::create();

	detector->detect(input, keypoints_orb);
	detector->compute(input, keypoints_orb, output_orb);

	drawKeypoints(input, keypoints_orb, output_orb, Scalar(0, 255, 0));
	imwrite(path + "_orb.jpg", output_orb);

	if (showOutput)
	{
		imshow("SIFT", output_sift);
		imshow("ORB", output_orb);
		waitKey(0);
	}
	cout << "SIFT output saved to " << path << "_sift.jpg;" << endl;
	cout << "ORB output saved to " << path << "_orb.jpg;" << endl;
	system("pause");
	return 0;
}