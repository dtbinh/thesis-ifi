/*
 * =====================================================================================
 *
 *       Filename:  testing.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  28/05/2014 10:43:16 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Quoc Khai, 
 *   Organization:  IFI
 *
 * =====================================================================================
 */
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/time.h>


#include "classificator.h"
#include "common.h"

#define NUMBER_OF_SCENE 10
#define TEST_LIBSVM "data/eval.libsvm"

typedef std::vector<int> row;
typedef std::vector<row> matrix;

int main(int argc, char **argv){
    if(argc < 2){
        std::cout << "usage: " << argv[0] << " test_file " << std::endl;
        exit(1);
    }

    std::vector<cv::Mat> testImages;
    cv::Mat testLabels(0, 1, CV_32FC1);
    std::vector<std::string> imageNames;
    readCsv(argv[1], testImages, testLabels, imageNames);
    
	std::string vocabularyFile = VOCABULARY_FILE;
	std::string svmFile = SVM_FILE;
    if(argc > 2){
        vocabularyFile = argv[2];
    }
    if(argc > 3){
        svmFile = argv[3];
    }
    
    int dictionarySize = DICTIONARY_SIZE;
    if(argc > 4){
        dictionarySize = atoi(argv[4]);
    }

    Classificator classificator(dictionarySize, vocabularyFile, svmFile);

    cv::Mat results(0, 1, CV_32FC1);

    matrix matriceConfusion(NUMBER_OF_SCENE); 
    for (unsigned int i = 0; i < NUMBER_OF_SCENE; i++) {
        std::vector<int> row(NUMBER_OF_SCENE); 
        matriceConfusion[i] = row;
    }

	cv::Mat evalData(0, DICTIONARY_SIZE, CV_32FC1);
	cv::Mat groundTruth(0, 1, CV_32FC1);
	std::vector<std::string> labels;
	
	std::cout<<"Evaluating classifier..."<<std::endl;
    struct timeval tim;        
    gettimeofday(&tim, NULL);
    double t1=tim.tv_sec+(tim.tv_usec/1000000.0);

//	classificator.extractBOWDescriptor(testImages);
//	evalData = classificator.getTrainingData();
//	classificator.saveLibSvm(TEST_LIBSVM, evalData, testLabels);
//	classificator.predicts(groundTruth);

    for(unsigned int i = 0; i < testImages.size(); i++){
		float result = classificator.predict(testImages[i]);
		groundTruth.push_back(result);
		//unsigned int pred_class = groundTruth.at<float>(i);
		unsigned int pred_class = result;
		unsigned int actual_class = testLabels.at<float>(i);
        matriceConfusion[pred_class - 1][actual_class - 1]++;
		
    }

    gettimeofday(&tim, NULL);
    double t2=tim.tv_sec+(tim.tv_usec/1000000.0);
    testLabels.convertTo(testLabels, CV_32FC1);
    double errorRate = (double) cv::countNonZero(testLabels - groundTruth) / groundTruth.rows;
    std::cout<<"Predict rate " << 100*(1 - errorRate) <<"%" << std::endl;
    std::cout<<"Predict in " << t2 -t1 <<"s"<<std::endl;
    printConfusionMatrix(matriceConfusion);
    return 0;
}
