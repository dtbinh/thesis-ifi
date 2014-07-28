/*
 * =====================================================================================
 *
 *       Filename:  sgdsvm.h
 *    Description:  
 *        Version:  1.0
 *        Created:  3/06/2014 10:06:38 AM
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  NGUYEN Quoc Khai, 
 *   Organization:  IFI
 *
 * =====================================================================================
 */



#ifndef _SGDSVM_H_
#define _SGDSVM_H_

#include <string>
#include <vector>
#include "WeightVector.h"

#define REST_FILE "data/rest.class"
#define TRAIN_FILE "data/train.binary"
#define TEST_FILE "data/test.binary"

typedef struct
{
	int id;
	std::string detail;
}ClassDetail;


//===========================================================	

class SgdSvm
{
public:
	SgdSvm();
	std::vector<ClassDetail> getClassDetail(std::string svm_file);
	void classifier(std::string train_file, std::string test_file);
	WeightVector trainBinaryClassificator(std::string data_filename, 
							 std::string model_filename,
							 double lambda, int max_iter, 
							 int exam_per_iter);
	void classifier(std::string train_file, std::string test_file,
					std::string model_filename,
					double lambda, int max_iter, 
					int exam_per_iter, int paral = 0);
	WeightVector* training(std::string train_file, std::string model_filename,
						   double lambda, int max_iter, int exam_per_iter,
						   std::vector<int> &classes);
	WeightVector* trainingParal(std::string train_file, std::string model_filename,
								double lambda, int max_iter, int exam_per_iter,
								std::vector<int> &classes);
	

//	int mcPredictor(std::vector<ClassDetail> classdetails, std::string model_filename,
//					double lambda, int max_iter, int exam_per_iter, double &error);
	
	
	

	
private:
};

typedef struct
{
	WeightVector wv;
	uint id;
}Weight;

	


#endif
