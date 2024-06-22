#ifndef histogram_hpp
#define histogram_hpp
#include <opencv2/opencv.hpp>

void showHistogram(cv::Mat src);
cv::Mat getHistogram(cv::Mat src);
#endif