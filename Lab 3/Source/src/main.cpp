#include <iostream>
#include <vector>
#include <opencv2/core/utils/logger.hpp>
#include <opencv2/core/utility.hpp>
#include "ultils.hpp"
#include <ctime>

// ref at https://docs.opencv.org/3.4/d0/d2e/classcv_1_1CommandLineParser.html
using cv::CommandLineParser;
using std::cout;
using std::endl;
using std::string;

int main(int argc, char **argv)
{
	// The logging level can be set with the following function
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

	const string keys =
		"{help h usage ? | | print this message }"
		"{e extract | | path to the folder contains images to extract features}"
		"{t train | | training mode }"
		"{k k_numb | 100 | number of clusters}";

	CommandLineParser parser(argc, argv, keys);

	if (parser.has("help"))
	{
		parser.printMessage();
		system("pause");
		return 0;
	}

	bool isTrain = parser.has("train");
	bool isExtract = parser.has("extract");

	if (!parser.check())
	{
		parser.printErrors();
		system("pause");
		return 0;
	}

	if (isExtract)
	{
		string imgPath = parser.get<string>("e");
		if (!parser.check())
		{
			parser.printErrors();
			system("pause");
			return 0;
		}
		cout << "Extracting features from " << imgPath << "\n";
		extractFeatures(imgPath);
	}

	if (isTrain)
	{
		string featurePath = parser.get<string>("train");
		int k = parser.get<int>("k");
		if (!parser.check())
		{
			parser.printErrors();
			system("pause");
			return 0;
		}
		cout << "Training with features from " << featurePath << " with k = " << k << endl;
		clock_t start = clock();
		train(featurePath, k);
		int raw_time = clock() - start;
		cout << "Training done in " << ((float)raw_time) / CLOCKS_PER_SEC << " s" << endl;
	}

	return 0;
}