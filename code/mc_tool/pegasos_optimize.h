// Distributed under GNU General Public License (see license.txt for details).
//
//  Copyright (c) 2007 Shai Shalev-Shwartz.
//  All Rights Reserved.
//=============================================================================
// File Name: pegasos_optimize.h
// header for the main optimization function of pegasos
//=============================================================================

#ifndef _SHAI_PEGASOS_OPTIMIZE_H
#define _SHAI_PEGASOS_OPTIMIZE_H

//*****************************************************************************
// Included Files
//*****************************************************************************
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <ctime>
#include <cmath>
#include "simple_sparse_vec_hash.h"
#include "WeightVector.h"


// function from Joachims that measures CPU time
double get_runtime(void);

void LearnReturnLast(// Input variables
		      std::vector<simple_sparse_vector>& Dataset,
		      std::vector<int>& Labels,
		      uint dimension,
		      std::vector<simple_sparse_vector>& testDataset,
		      std::vector<int>& testLabels,
		      double lambda,int max_iter,
		      int exam_per_iter,
		      std::string& model_filename,
		      // Output variables
		      double& train_time,double& calc_obj_time,
		      double& obj_value,double& norm_value,
		      double& loss_value,double& zero_one_error,
		      double& test_loss,double& test_error,
		      // additional parameters
		      int eta_rule_type , double eta_constant ,
		      int projection_rule, double projection_constant);

WeightVector getWeightVector(// Input variables
    std::vector<simple_sparse_vector>& Dataset,
	std::vector<int>& Labels,
	uint dimension,
	double lambda,int max_iter,
	int exam_per_iter,
	// additional parameters
	int eta_rule_type , double eta_constant ,
	int projection_rule, double projection_constant);

WeightVector getWeightVectorBlanced(// Input variables
    std::vector<simple_sparse_vector>& Dataset,
	std::vector<int>& Labels,
	const std::vector<int>& pos_index_list,
	const std::vector<int>& neg_index_list,
	uint dimension,
	double lambda,int max_iter,
	int exam_per_iter,
	// additional parameters
	int eta_rule_type , double eta_constant ,
	int projection_rule, double projection_constant);


WeightVector getWeightVectorBlancedNew(// Input variables
    std::vector<simple_sparse_vector>& Dataset,
	std::vector<int>& Labels,
	std::vector<int>& pos_index_list,
	std::vector<int>& neg_index_list,
	uint dimension,
	double lambda,int max_iter,
	int exam_per_iter,
	// additional parameters
	int eta_rule_type , double eta_constant ,
	int projection_rule, double projection_constant);


// function for reading the data
void ReadData(// input
	      std::string& data_filename,
	      // output
	      std::vector<simple_sparse_vector> & Dataset,
	      std::vector<int> & Labels,
	      uint& dimension,
	      double& readingTime);


// function for running experiments
void run_experiments(std::string& experiments_file,
		     std::vector<simple_sparse_vector>& Dataset,
		     std::vector<int>& Labels,
		     uint dimension,
		     std::vector<simple_sparse_vector>& testDataset,
		     std::vector<int>& testLabels);


#endif
