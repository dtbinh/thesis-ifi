/*
 * =====================================================================================
 *
 *       Filename:  classificator.h
 *    Description:  
 *        Version:  1.0
 *        Created:  28/05/2014 10:06:38 AM
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  NGUYEN Quoc Khai, 
 *   Organization:  IFI
 *
 * =====================================================================================
 */

#include <vector>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/ml/ml.hpp"


#ifndef _CLASSIFICATOR_H
#define _CLASSIFICATOR_H

#define debug(str)								\
    std::cout<<str<<std::endl;

class Classificator{
public:
	Classificator();
	Classificator(int dictionarySize);
	Classificator(int dictionarySize, std::string vocabularyFile, std::string svmFile);
	cv::Mat getTrainingData(){
		return this->trainingData;
	}
	
	void train(const std::vector<cv::Mat> &images, const cv::Mat &lbs, 
			   double c = 300.0, double gamma = 0.5);
	float predict(const cv::Mat &image);
	void predicts(cv::Mat &result);
	void save(std::string vocabularyFile, std::string svmFile);
	void saveLibSvm(std::string file_name, const cv::Mat &training_data, 
					const cv::Mat &labels);
	void extractDescriptor(std::string image_name, std::string key_name);
	void extractDescriptor(const std::vector<cv::Mat> &images);
	void cluster();
	void buildHistogram(const std::vector<cv::Mat> &images);
	void extractBOWDescriptor(const std::vector<cv::Mat> &images);

private:

	int dictionarySize;
	cv::Mat labels;
	cv::Mat trainingData;
	std::vector<cv::Mat> descriptors;
	cv::BOWKMeansTrainer *bowTrainer;
	cv::BOWImgDescriptorExtractor *bowExtractor;


	cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
	cv::Ptr<cv::DescriptorMatcher> descriptorMatcher;
        
	cv::Ptr<cv::FeatureDetector> featureDetector;

	//store keypoint to avoid recompute
	std::vector<std::vector<cv::KeyPoint> > trainKeypoints;
	cv::Mat vocabulary;
	//svm and it's params
	cv::SVM svm; 
	CvSVMParams params;
};
#endif
