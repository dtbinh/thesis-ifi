/*
 * =====================================================================================
 *       Filename:  Common.h
 *    Description:  
 *        Version:  1.0
 *        Created:  28/05/2014 06:10:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Quoc Khai, 
 *   Organization:  IFI
 * =====================================================================================
 */
#ifndef _COMMON_H_
#define _COMMON_H_

#include <vector>
#include <iostream>

#include <fstream>
#include <sstream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#define DICTIONARY_SIZE 2000
#define VOCABULARY_FILE "data/vocabulary.xml" 
#define SVM_FILE "data/svm.xml"

void readCsv(char *fileName, std::vector<cv::Mat>& images, 
			 cv::Mat& labels, char separator = ';') 
{
    std::ifstream file(fileName, std::ifstream::in);
    if (!file) 
	{
        std::string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    std::string line, path, classlabel;
    while (getline(file, line)) 
	{
        std::stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) 
		{
            images.push_back(cv::imread(path, 0));
            labels.push_back((float)atoi(classlabel.c_str()));
        }
    }
}

void readCsv(char *fileName, std::vector<cv::Mat>& images, cv::Mat& labels, 
			 std::vector<std::string> &imageNames ,char separator = ';') 
{
    std::ifstream file(fileName, std::ifstream::in);
    if (!file) 
	{
        std::string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    std::string line, path, classlabel;
    while (getline(file, line)) 
	{
        std::stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) 
		{
            images.push_back(cv::imread(path, 0));
            labels.push_back((float)atoi(classlabel.c_str()));
            imageNames.push_back(path);
        }
    }
}

static double getPrecision(const std::vector<std::vector<int> > &cm, int position)
{
	int fp = 0;
	int tp = cm[position][position];
	int n = cm[0].size();
	for (int i = 0; i < n; i++)
	{
		if(i != position)
		{
			fp += cm[i][position];
		}
	}
	return 100.0*tp/(tp + fp);
}
  
double getRecall(const std::vector<std::vector<int> > &cm, int position){
	int fn = 0;
	int tp = cm[position][position];
	int n = cm[0].size();
	for (int i = 0; i < n; i++){
		if(i != position){
			fn += cm[position][i];
		}
	}
	return 100.0*tp/(tp + fn);
}

void printConfusionMatrixLatex(const std::vector<std::vector<int> > &cm)
{
    std::ofstream ofs("data/cf.tex");  
    ofs.precision(1);
    ofs.unsetf ( std::ios::floatfield );
	//Affichage de la matrice de confusion
    std::cout << std::endl<<"Matrice de confusion"<<std::endl;
	//header line of matrix
	ofs<<"&"<<std::fixed;
    for (unsigned i = 0; i < cm.size(); i++) 
	{
		ofs<<i + 1<<"&";
	}
  
	ofs<<"r\\\\"<< std::endl<<"\\hline"<<std::endl;
    for (unsigned i = 0; i < cm.size(); i++) 
	{
		//first column
		ofs<<i + 1<<"&";
        for (unsigned int j = 0; j < cm.size(); j++) {
			ofs<<cm[i][j]<<"&";
		}
		ofs<<getRecall(cm, i)<<"\\\\"<< std::endl<<"\\hline"<<std::endl;
	}
  
	ofs<<"p&"<<std::fixed;
	//print precision
    for (unsigned i = 0; i < cm.size(); i++) {
		ofs<<getPrecision(cm,i)<<"&";
	}
  
	ofs<<"\\\\"<< std::endl<<"\\hline"<<std::endl;
	std::cout<<std::endl;
	ofs.close();
}

void printConfusionMatrix(const std::vector<std::vector<int> > &cm){
	debug("cf size");
	debug(cm.size());
    std::ofstream ofs("data/cf");  
    ofs.precision(1);
    ofs.unsetf ( std::ios::floatfield );
	//Affichage de la matrice de confusion
    std::cout << std::endl<<"Matrice de confusion"<<std::endl;
	//header line of matrix
	ofs<<"\t"<<std::fixed;
    for (unsigned i = 0; i < cm.size(); i++) {
		ofs<<i + 1<<"\t";
	}
	ofs<<"r"<< std::endl;
    for (unsigned i = 0; i < cm.size(); i++) {
		//first column
		ofs<<i + 1<<"\t";
        for (unsigned int j = 0; j < cm.size(); j++) {
			ofs<<cm[i][j]<<"\t";
		}
		ofs<<getRecall(cm, i)<<"\t"<<std::endl <<std::endl;
	}
	ofs<<"p\t"<<std::fixed;
	//print precision
    for (unsigned i = 0; i < cm.size(); i++) {
		ofs<<getPrecision(cm,i)<<"\t";
	}
	ofs.close();
}

#endif
