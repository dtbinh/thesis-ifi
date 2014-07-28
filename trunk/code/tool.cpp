#include "tool.h"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/ml/ml.hpp"
#include <opencv/cv.hpp>
#include <stdio.h>
#include <fstream>

#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>


Tool::Tool()
{
}

Tool::~Tool()
{
}

std::vector<cv::KeyPoint> Tool::getKeyPoints(cv::Mat image)
{
//To store the keypoints that will be extracted by SIFT
	std::vector<cv::KeyPoint> keypoints;
//The SIFT feature extractor and descriptor
	cv::SiftFeatureDetector detector;

//Detect keypoint for image <image>
	detector.detect(image, keypoints);
	std::cout<<"Nombre de key point: " <<keypoints.size() <<std::endl;
	return keypoints;
}

cv::Mat Tool::getImageKeyPoint(cv::Mat image)
{
	cv::Mat image_keypoint;
//To store the keypoints that will be extracted by SIFT
	std::vector<cv::KeyPoint> keypoints;
	keypoints = this->getKeyPoints(image);
	cv::drawKeypoints(image, keypoints, image_keypoint, 
					  cv::Scalar::all(-1), cv::DrawMatchesFlags::DEFAULT);
	return image_keypoint;
}

cv::Mat Tool::getDescriptor(cv::Mat image)
{
	cv::Mat descriptor;
//To store the keypoints that will be extracted by SIFT
	std::vector<cv::KeyPoint> keypoints;
//The SIFT feature extractor and descriptor
	cv::SiftFeatureDetector detector;

	keypoints = this->getKeyPoints(image);

	std::ofstream ofs_keypoint("images/img1_keypoints.OUT");
	for(unsigned int i = 0; i < keypoints.size(); i++)
	{
		cv::Point2f p = keypoints[i].pt;
		ofs_keypoint<<p.x <<", " <<p.y <<": " <<keypoints[i].size <<"\n";
	}
	

	detector.compute(image, keypoints,descriptor);

	std::ofstream ofs("images/img1_descriptor.OUT");
	unsigned int M = descriptor.rows;
	unsigned int N = descriptor.cols;
	for(unsigned int i = 0; i < M; i++)
	{
		for(unsigned int j = 0; j < N; j++)
		{
			ofs<<(int)descriptor.at<uchar>(i, j) <<" ";
		}
		ofs<<std::endl <<std::endl;
	}
	

	char * imageTag = new char[10];
	sprintf(imageTag,"img1");

	cv::FileStorage fs1("images/descriptor.yml", cv::FileStorage::WRITE);
	fs1<<imageTag <<descriptor;
	fs1.release();
	return descriptor;
}

