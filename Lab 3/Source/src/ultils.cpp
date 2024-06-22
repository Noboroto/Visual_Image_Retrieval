#include "ultils.hpp"
#include "histogram.hpp"
#include "SaveBinaryCV.h"

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core.hpp>

#include <vector>
#include <iostream>
#include <fstream>

using cv::glob;
using cv::kmeans;
using cv::KMEANS_PP_CENTERS;
using cv::Mat;
using cv::readMatBinary;
using cv::TermCriteria;
using cv::writeMatBinary;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::vector;

// File structure
// 1st bit: H, C, S, O: Histogram, Correlogram, SIFT, ORB

void extractHistogram(const vector<Mat> &images, const vector<string> &files)
{
	string name = "Histograms";
	Mat histograms;
	int total = (int)images.size();
	cout << "Extracting " << name << "...\n";
	for (int i = 0; i < total; i++)
	{
		Mat input;
		cv::cvtColor(images[i], input, cv::COLOR_BGR2GRAY);
		Mat hist = getHistogram(input);
		histograms.push_back(hist.t());
		cout << "Image " << i << "/" << total << " processed\n";
		cout << "Path: " << files[i] << endl;
	}
	ofstream ofs(name + ".bin", std::ios::binary);
	char mode = 'H';
	ofs.write((char *)&mode, sizeof(char));
	ofs.write((char *)&total, sizeof(int));
	ofs.flush();
	writeMatBinary(ofs, histograms);
	ofs.close();
	cout << "Extract " << name << " done\n";
}

void extractSIFT(const vector<Mat> &images, const vector<string> &files)
{
	int total = (int)images.size();
	char mode = 'S';
	string name = "SIFT";

	vector<int> sizes = vector<int>(total, 0);

	cout << "Extracting " << name << " features...\n";
	cv::Ptr<cv::Feature2D> sift = cv::SIFT::create();
	Mat descriptors;

	ofstream features_bin(name + ".bin", std::ios::binary);
	features_bin.write((char *)&mode, sizeof(char));
	features_bin.write((char *)&total, sizeof(int));
	features_bin.write((char *)&sizes[0], sizeof(int) * total);
	features_bin.flush();

	for (int i = 0; i < images.size(); i++)
	{
		Mat input = images[i];
		Mat gray;
		cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
		vector<cv::KeyPoint> kps;
		Mat des;
		sift->detectAndCompute(gray, Mat(), kps, des);
		descriptors.push_back(des);
		cout << "Image " << i << "/" << images.size() << " processed\n";
		cout << "Path: " << files[i] << endl;

		sizes[i] = des.rows;
	}
	writeMatBinary(features_bin, descriptors);

	features_bin.seekp(sizeof(char) + sizeof(int));
	features_bin.write((char *)&sizes[0], sizeof(int) * total);

	features_bin.close();
	cout << "Extract " << name << " features done\n";
}

void extractFeatures(string folderPath)
{
	// get the list of images in the folder
	vector<string> files;
	glob(folderPath, files);

	// read the images
	vector<Mat> images;
	for (auto &file : files)
	{
		Mat img = cv::imread(file);
		if (img.empty())
		{
			cout << "Could not read the image: " << file << std::endl;
			continue;
		}
		images.push_back(img);
	}

	extractHistogram(images, files);
	extractSIFT(images, files);
}

void train(string filePath, int k_clusters)
{
	// check if file exists, C++17
	std::ifstream file(filePath);
	char mode = 'N';
	if (!file.good())
	{
		cout << "File not found\n";
		file.close();
		return;
	}
	else
	{
		cout << "Reading file..." << endl;
		file.read(&mode, sizeof(char));
	}

	const int iterationNumber = (int)1e2;
	const int attempts = 3;

	Mat data;
	Mat visual_words;
	Mat labels;
	string name = "None";
	int total = 0;
	file.read((char *)&total, sizeof(total));

	cout << "Training...\n";
	switch (mode)
	{
	case 'H':
		// training kmean with histograms
		name = "Histograms";
		cout << "K-mean " << name << endl;
		readMatBinary(file, data);
		file.close();
		kmeans(data, k_clusters, labels, TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, iterationNumber, 1e-4), attempts, KMEANS_PP_CENTERS, visual_words);
		break;

	case 'S':
		// training kmean with sift descriptors
		name = "SIFT";
		cout << "K-mean " << name << endl;
		readMatBinary(file, data);
		file.close();
		kmeans(data, k_clusters, labels, TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, iterationNumber, 1e-4), attempts, KMEANS_PP_CENTERS, visual_words);
		break;
	}
	cv::FileStorage fs(name + "_kmean.json", cv::FileStorage::WRITE);
	fs.write("name", name);
	fs.write("total", total);
	fs.write("k_clusters", k_clusters);
	fs.write("visual_words", visual_words);
	fs.release();

	ofstream ofs(name + "_kmean.res", std::ios::binary);
	ofs.write((char *)&mode, sizeof(char));
	ofs.write((char *)&k_clusters, sizeof(int));
	writeMatBinary(ofs, visual_words);
	cout << "Training done\n";
}