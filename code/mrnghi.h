#include <vector>
#include <iostream>
#include <fstream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/ml/ml.hpp"



//////////////////////////////////////////////////////////////////////////
// References: DO Thanh Nghi
//
// Write Mat to libsvm format file 
//
// label 1: idx_i:val_i ... 
//  ...
// label n: idx_i:val_i ...                	 
//
//////////////////////////////////////////////////////////////////////////
void mat_svm_file(std::string fn, cv::Mat m, std::vector<std::string> labels) {
	std::ofstream os(fn.c_str());
	int cols = m.cols, rows = m.rows;
	for(int i=0; i<rows; i++) {
		const float* row = m.ptr<float>(i);
		os << labels[i];
		//cout << labels[i] << endl;
		for(int j=0; j<cols; j++)
			if(row[j] != 0)
				os << " " << (j+1) << ":" << row[j];
		os << std::endl;
	}
}


//////////////////////////////////////////////////////////////////////////
// REference: DO Thanh Nghi
// Load file to Mat
//
// nb_rows nb_cols
// row 1: val 1 ... val k
//  ...
// row n: val 1 ... val k                	 
//
//////////////////////////////////////////////////////////////////////////

cv::Mat loadMat(std::string fn) {
	std::ifstream is(fn);
	unsigned int cols, rows;

	is >> rows; is >> cols;

	cv::Mat ret(rows, cols, CV_32FC1);
	std::cout << "Loading mat...:" << rows << " x " << cols << std::endl;
	for(unsigned int i=0; i<rows; i++) {
		for(unsigned int j=1; j<cols; j++)
			is >> ret.at<float>(i,j);
	}

	return ret;
}



//////////////////////////////////////////////////////////////////////////
// REference: DO Thanh Nghi
// Write Mat to file 
//
// nb_rows nb_cols
// row 1: val 1 ... val k
//  ...
// row n: val 1 ... val k                	 
//
//////////////////////////////////////////////////////////////////////////

void saveMat(std::string fn, const cv::Mat &m) {
	std::ofstream os(fn);
	unsigned int cols = m.cols, rows = m.rows;
	os << rows << " " << cols << std::endl;
	for(unsigned int i=0; i<rows; i++) {
		const float* row = m.ptr<float>(i);
		os << row[0];
		for(unsigned int j=1; j<cols; j++)
			os << " " << row[j];
		os << std::endl;
	}
}
