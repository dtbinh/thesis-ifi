/*
 * =====================================================================================
 *
 *       Filename:  sgdsvm.cpp
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

#include "sgdsvm.h"
#include "common.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "simple_sparse_vec_hash.h"
#include "pegasos_optimize.h"
#include <omp.h>

SgdSvm::SgdSvm()
{
}

std::vector<ClassDetail> SgdSvm::getClassDetail(std::string svm_file)
{
	std::ifstream ifs(svm_file.c_str());
	std::string line;
	ClassDetail detail;
	std::vector<ClassDetail> classdetails;
	uint i = 0;	
	while(std::getline(ifs, line))
	{
		std::string::size_type l = line.find(' ');
		std::string _id = line.substr(0, l);
		if(_id != "#")
		{	
			detail.id = atoi(_id.c_str());
			detail.detail = line.substr(l, line.length() - 1);
			classdetails.push_back(detail);
			i++;
		}
	}
	return classdetails;
}

std::vector<int> getClasses(std::vector<ClassDetail> clds)
{
	std::vector<int> classes;
	int id = -1000;
	uint N = clds.size();
	
	std::vector<int> classes_temp;
	

	for(uint i = 0; i < N; i++)
	{
		classes_temp.push_back(clds[i].id);
	}
	std::sort(classes_temp.begin(), classes_temp.end());
	
	for(uint i = 0; i < N; i++)
	{
		if(id != classes_temp[i])
		{
			id = classes_temp[i];
		    classes.push_back(id);
		}
	}
	return classes;
}

std::vector<int> getClasses(std::vector<int> cls)
{
	std::vector<int> classes;
	int id = -1000;
	uint N = cls.size();
	
	std::sort(cls.begin(), cls.end());
	
	for(uint i = 0; i < N; i++)
	{
		if(id != cls[i])
		{
			id = cls[i];
		    classes.push_back(id);
		}
	}
	return classes;
}

void SgdSvm::classifier(std::string train_file, std::string test_file,
						std::string model_filename,
						double lambda, int max_iter, 
						int exam_per_iter, int paral)
{
//training start
	long start_time = get_runtime();
	std::cout<<"training start..............\n";
	std::vector<int> classes;

//get n models trained (one vs all)
    WeightVector* ws;

	if(paral == 1)
		ws = this->trainingParal(train_file, model_filename, 
								 lambda, max_iter, 
								 exam_per_iter, classes);
	else
		ws = this->training(train_file, model_filename, 
							lambda, max_iter, 
							exam_per_iter, classes);


	uint nb_model = classes.size();

	std::cout<<"training finish!\n";
	long end_time = get_runtime();
	long _time = end_time - start_time;
	std::cout<<"time for training: " <<_time <<"s\n";	
//training finish!


//prepare for test data
	uint test_dimension = 0;
	std::vector<simple_sparse_vector> testDataset;
	std::vector<int> testLabels;
	long testReadingTime;
	if (test_file != "noTestFile") {
		ReadData(test_file,testDataset,testLabels,test_dimension,testReadingTime);
	} else {
		testReadingTime = 0;
	}

	// Calculate test_loss and test_error
	double test_loss = 0.0;
	double test_error = 0.0;
	uint nb_test_exam = testDataset.size();
	uint nb_true = 0;
	uint **cf;
	//allocation for matrix
	cf = new uint*[nb_model];
	for(uint i = 0; i < nb_model; i++)
	{
		cf[i] = new uint[nb_model];
	}
	//init for matrix
	for(uint i = 0; i < nb_model; i++)
		for(uint j = 0; j < nb_model; j++)
			cf[i][j] = 0;	
	
	for (uint i=0; i < nb_test_exam; ++i) 
	{
		int real_class = std::find(classes.begin(), 
								   classes.end(), 
								   testLabels[i]) - classes.begin();
		double min_cur_loss = 100000.0;
		int pred_class = 1;
		//std::cout<<"=============exam " <<i <<"===========\n";
		for(uint m = 0; m < nb_model; m++)
		{
			//std::cout<<"model " <<m <<"\n";			
			double cur_loss = 1 - (ws[m] * testDataset[i]);
			if(cur_loss < min_cur_loss)
			{
				pred_class = m;
				min_cur_loss = cur_loss;
			}
			
		}
		
		cf[real_class][pred_class]++;
		if(real_class == pred_class)
			nb_true++;
		
		//std::cout<<"Real: " <<real_class <<"\nPredict:" <<pred_class <<"\n";
		//std::cout<<"Cur loss: " <<min_cur_loss <<"\n";
	}
	std::cout<<"==================RESULTAT====================\n"
			 <<"Predict rate: " <<100*((double)nb_true/(double)nb_test_exam) <<"%\n"
			 <<"Confusion matrix\n";
	

	for(uint i = 0; i < nb_model; i++)
	{
		for(uint j = 0; j < nb_model; j++)
			std::cout<<cf[i][j] <<"\t";
		std::cout<<"\n";
	}

	if (testDataset.size() != 0) {
		test_loss /= testDataset.size();
		test_error /= testDataset.size();
	}
}

WeightVector* SgdSvm::training(std::string train_file, 
							   std::string model_filename,
							   double lambda, int max_iter, 
							   int exam_per_iter, 
							   std::vector<int> &classes)
{
	// -------------------------------------------------------------
	// ---------------------- read the data ------------------------
	// -------------------------------------------------------------
	std::string experiments_file = "noExperimentsFile";
	uint dimension = 0;
	std::vector<simple_sparse_vector> Dataset;

//multi class labels
	std::vector<int> Labels;
	long readingTime;
	ReadData(train_file, Dataset, Labels, dimension, readingTime);
	std::cout<<"time for reading the data: " <<readingTime <<"s\n";

//prepare for training data
	uint nb_exam = Dataset.size();
	classes = getClasses(Labels);
	uint nb_class = classes.size();
	
	std::vector < std::vector<int> > labels_list;
	std::vector<int> _labels = Labels;

	std::vector < std::vector<int> > pos_index_list;
	std::vector < std::vector<int> > neg_index_list;
	std::vector<int> pos_index;
	std::vector<int> neg_index;

	for(uint c = 0; c < nb_class; c++)
	{
		int class_id = classes[c];
//binary labels: changed each model
		for(uint i = 0; i < nb_exam; i++)
		{
			if(class_id == Labels[i])
			{
				pos_index.push_back(i);
			    _labels[i] = 1;
			} 
			else {
				neg_index.push_back(i);
				_labels[i] = -1;
			}
		}
		labels_list.push_back(_labels);
		pos_index_list.push_back(pos_index);
		neg_index_list.push_back(neg_index);
	}

    WeightVector* ws = new WeightVector[nb_class];
	for(uint c = 0; c < nb_class; c++)
	{
		long start_t = get_runtime();


		WeightVector w = getWeightVector(Dataset, labels_list[c], dimension, lambda, 
										 max_iter, exam_per_iter, 0,0.0,0,0.0);

/*
		WeightVector w = getWeightVectorBlanced(Dataset, labels_list[c], pos_index_list[c],
												neg_index_list[c], dimension, lambda, 
												max_iter, exam_per_iter,0,0.0,0,0.0);
*/
/*
		WeightVector w = getWeightVectorBlancedNew(Dataset, labels_list[c], pos_index_list[c],
												   neg_index_list[c], dimension, lambda, 
												   max_iter, exam_per_iter,0,0.0,0,0.0);
*/


		ws[c] = w;
		long end_t = get_runtime();
		long t = end_t - start_t;
		std::cout<<"time for train model " <<c <<": " <<t <<"s\n";
	}
	
	std::ofstream ofs("ws.out");
	
	for(uint i = 0; i < nb_class; i++)
		ws[i].print(ofs);
	

	return ws;
}

WeightVector* SgdSvm::trainingParal(std::string train_file, 
		std::string model_filename,
		double lambda, int max_iter, 
		int exam_per_iter, 
		std::vector<int> &classes)
{
	// -------------------------------------------------------------
	// ---------------------- read the data ------------------------
	// -------------------------------------------------------------
	std::string experiments_file = "noExperimentsFile";
	uint dimension = 0;
	std::vector<simple_sparse_vector> Dataset;

//multi class labels
	std::vector<int> Labels;
	long readingTime;
	ReadData(train_file, Dataset, Labels, dimension, readingTime);
	std::cout<<"time for reading the data: " <<readingTime <<"s\n";

//prepare for training data
	uint nb_exam = Dataset.size();
	classes = getClasses(Labels);
	uint nb_class = classes.size();

	
	std::vector < std::vector<int> > labels_list;
	std::vector < std::vector<int> > pos_index_list;
	std::vector < std::vector<int> > neg_index_list;
	std::vector<int> _labels = Labels;
	std::vector<int> pos_index;
	std::vector<int> neg_index;
	
	for(uint c = 0; c < nb_class; c++)
	{
		int class_id = classes[c];
//binary labels: changed each model
		for(uint i = 0; i < nb_exam; i++)
		{
			if(class_id == Labels[i])
			{
				pos_index.push_back(i);
			    _labels[i] = 1;
			} 
			else {
				neg_index.push_back(i);
				_labels[i] = -1;
			}
		}
		labels_list.push_back(_labels);
		pos_index_list.push_back(pos_index);
		neg_index_list.push_back(neg_index);
	}

	WeightVector* ws = new WeightVector[nb_class];

#pragma omp parallel for
	for(uint c = 0; c < nb_class; c++)
	{
		long start_t = get_runtime();

		WeightVector w = getWeightVector(Dataset, labels_list[c], dimension, lambda, 
										 max_iter, exam_per_iter,0,0.0,0,0.0);

/*
		WeightVector w = getWeightVectorBlanced(Dataset, labels_list[c], pos_index_list[c],
												neg_index_list[c], dimension, lambda, 
												max_iter, exam_per_iter,0,0.0,0,0.0);
*/
/*
		WeightVector w = getWeightVectorBlancedNew(Dataset, labels_list[c], pos_index_list[c],
												   neg_index_list[c], dimension, lambda, 
												   max_iter, exam_per_iter,0,0.0,0,0.0);
*/
		
	    ws[c] = w;
		long end_t = get_runtime();
		long t = end_t - start_t;
		std::cout<<"time for train model " <<c <<": " <<t <<"s\n";
	}

	std::ofstream ofs("ws.out");
	
	for(uint i = 0; i < nb_class; i++)
		ws[i].print(ofs);
	

	return ws;
}
