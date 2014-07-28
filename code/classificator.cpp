/*
 * =================================================================================
 *
 *       Filename:  classificator.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  28/05/2014 09:56:23 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Quoc Khai, 
 *   Organization:  IFI
 *
 * =================================================================================
 */

#include "classificator.h"
#include "mrnghi.h"
#include <sys/time.h>
#include <fstream>

#define ATTEMPS 2
#define GAMMA 0.5 
#define C_VALUE 300.0
#define DESCRIPTOR_FILE "data/descriptor.OUT"
#define LIBSVM_FILE "data/train.libsvm"
#define VOCABULARY_TXT_FILE "data/vocabulary.OUT"
#define DICTIONARY_SIZE 2000


Classificator::Classificator()
	:descriptorExtractor(new cv::SiftDescriptorExtractor),
	 featureDetector(new cv::SiftFeatureDetector())
{
}

Classificator::Classificator(int size)
	: dictionarySize(size),
	  descriptorExtractor(new cv::SiftDescriptorExtractor),
	  descriptorMatcher(cv::DescriptorMatcher::create("FlannBased")),
	  featureDetector(new cv::SiftFeatureDetector())
{
    bowTrainer = new cv::BOWKMeansTrainer(dictionarySize,
										  cv::TermCriteria( CV_TERMCRIT_ITER | 
															CV_TERMCRIT_EPS, 10, 0.01), 
										  ATTEMPS, cv::KMEANS_PP_CENTERS);
	bowExtractor = new cv::BOWImgDescriptorExtractor(descriptorExtractor, 
													 descriptorMatcher);
	this->trainingData = cv::Mat(0, DICTIONARY_SIZE, CV_32FC1);
}

Classificator::Classificator(int dictionarySize, std::string vocabularyFile,
							 std::string svmFile)
	:dictionarySize(dictionarySize),
	 descriptorExtractor(new cv::SiftDescriptorExtractor),
	 descriptorMatcher(cv::DescriptorMatcher::create("FlannBased")),
	 featureDetector(new cv::SiftFeatureDetector())
{
    bowTrainer = new cv::BOWKMeansTrainer(dictionarySize,
										  cv::TermCriteria( CV_TERMCRIT_ITER | 
															CV_TERMCRIT_EPS, 10, 0.01), 
										  ATTEMPS, cv::KMEANS_PP_CENTERS);
	bowExtractor = new cv::BOWImgDescriptorExtractor(descriptorExtractor, 
													 descriptorMatcher);
    cv::FileStorage fs(vocabularyFile, cv::FileStorage::READ);
    fs["Vocabulary"]>>vocabulary;
    bowExtractor->setVocabulary(vocabulary);
    svm.load(svmFile.c_str());
	this->trainingData = cv::Mat(0, DICTIONARY_SIZE, CV_32FC1);
}

void Classificator::train(const std::vector<cv::Mat> &images, const cv::Mat &lbs, 
						  double c, double gamma)
{
	this->extractDescriptor(images);
	this->cluster();
	this->buildHistogram(images);
	this->saveLibSvm(LIBSVM_FILE, this->trainingData, lbs);
	

    //setup parammeters for svm
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::RBF;
    //params.kernel_type = CvSVM::LINEAR;
    params.gamma = gamma;
    params.C = c;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 1e-6);
    //train using SVM
    svm.train(trainingData, lbs, cv::Mat(), cv::Mat(), params);
}

float Classificator::predict(const cv::Mat &image)
{
    std::vector<cv::KeyPoint> keypoint;
    featureDetector->detect(image, keypoint);
    cv::Mat bowDescriptorOfTestImage;
    bowExtractor->compute(image, keypoint, bowDescriptorOfTestImage);
    return svm.predict(bowDescriptorOfTestImage);
}

void Classificator::predicts(cv::Mat &result)
{
/*
	uint N = this->descriptors.size();
	for(uint i = 0; i < N; i++)
	{
		float res = svm.predict(this->descriptors[i]);
		result.push_back(res);
	}
*/
	svm.predict(this->trainingData, result);
}
		

void Classificator::save(std::string vocabularyFile, std::string svmFile)
{
	saveMat(VOCABULARY_TXT_FILE, vocabulary);
    cv::FileStorage fs(vocabularyFile, cv::FileStorage::WRITE);
    fs<<"Vocabulary"<<vocabulary;
    svm.save(svmFile.c_str());
}

void Classificator::saveLibSvm(std::string file_name, const cv::Mat &training_data, 
							   const cv::Mat &labels)
{
	uint M = labels.rows;
	std::vector<std::string> lbs;
	for(uint i = 0; i < M; i++)
	{
		std::stringstream ss;
		ss<<labels.at<float>(i, 0);
		lbs.push_back(ss.str());
	}
	
	
	mat_svm_file(file_name, training_data, lbs);
}


void writeDescriptors(const std::vector<cv::Mat> &descriptors)
{
	std::ofstream ofs(DESCRIPTOR_FILE);
	unsigned SIZE = descriptors.size();
	unsigned int NB_DES = 0;
	unsigned int NB_DEM = 0;
	if(SIZE > 0)
		NB_DEM = descriptors[0].cols;

	for(unsigned int d = 0; d < SIZE; d++)
	{
		cv::Mat descriptor = descriptors[d];
		unsigned int M = descriptor.rows;
		NB_DES += M;
	}
	ofs<<NB_DEM <<"\n";
	ofs<<NB_DES <<"\n";

	for(unsigned int d = 0; d < SIZE; d++)
	{
		cv::Mat descriptor = descriptors[d];
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
	}
}

void writeDescriptor(const cv::Mat &descriptor, std::string des_name)
{
	std::ofstream ofs(des_name.c_str());

	unsigned int M = descriptor.rows;
	unsigned int N = descriptor.cols;
	ofs<<N <<std::endl;
	ofs<<M <<std::endl;
	
	for(unsigned int i = 0; i < M; i++)
	{
		for(unsigned int j = 0; j < N; j++)
		{
			ofs<<(int)descriptor.at<uchar>(i, j) <<" ";
		}
		ofs<<std::endl;
	}
}

void Classificator::extractDescriptor(const std::vector<cv::Mat> &images)
{
    struct timeval tim;        
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	debug("Extracting descriptors ...");

    for (unsigned int i = 0; i < images.size(); i++){
        std::vector<cv::KeyPoint> keypoint;
        featureDetector->detect(images[i], keypoint);
        trainKeypoints.push_back(keypoint);
        cv::Mat features;
        descriptorExtractor->compute(images[i], keypoint, features);
		this->descriptors.push_back(features);
        bowTrainer->add(features);
    }
    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	std::cout<<"Extracting in " <<t2 - t1 <<"s" <<std::endl;
}

void Classificator::extractDescriptor(std::string image_name, std::string key_name)
{
	cv::Mat image = cv::imread(image_name, 0);
	std::vector<cv::KeyPoint> keypoint;
	featureDetector->detect(image, keypoint);
	cv::Mat features;
	descriptorExtractor->compute(image, keypoint, features);
	writeDescriptor(features, key_name);
}

void Classificator::cluster()
{
    struct timeval tim;        
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	debug("Begin clustering ...");

    //dictionary is centroids 
    vocabulary = bowTrainer->cluster();
    bowExtractor->setVocabulary(vocabulary);
    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	std::cout<<"Clustered in " <<t2 - t1 <<"s" <<std::endl;
}

void Classificator::buildHistogram(const std::vector<cv::Mat> &images)
{
    struct timeval tim;        
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	debug("Build histograms ...");
    //loop through train data's key point to compute historgram of words    
    for (unsigned int i = 0; i < images.size(); i++){
        cv::Mat bowDescriptor;
		cv::Mat features;
        bowExtractor->compute(images[i], trainKeypoints[i], bowDescriptor);
        descriptorExtractor->compute(images[i], trainKeypoints[i], features);
		bowDescriptor *= features.rows;
        trainingData.push_back(bowDescriptor);
    }

    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	std::cout<<"build histograms in " <<t2 - t1 <<"s" <<std::endl;
}

void Classificator::extractBOWDescriptor(const std::vector<cv::Mat> &images)
{
    struct timeval tim;        
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);
	debug("Build histograms ...");
    //loop through train data's key point to compute historgram of words    
    for (unsigned int i = 0; i < images.size(); i++){
        cv::Mat bowDescriptor;
		cv::Mat features;
        std::vector<cv::KeyPoint> keypoint;
        featureDetector->detect(images[i], keypoint);
        bowExtractor->compute(images[i], keypoint, bowDescriptor);
        descriptorExtractor->compute(images[i], keypoint, features);
		bowDescriptor *= features.rows;
        trainingData.push_back(bowDescriptor);
    }

    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
	std::cout<<"build histograms in " <<t2 - t1 <<"s" <<std::endl;
}

