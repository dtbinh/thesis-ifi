#include "sgdsvm.h"
#include <iostream>
#include "cmd_line.h"

int main(int argc, char* argv[])
{
/*
	if(argc < 3)
		std::cout<<"Usage: " <<argv[0] <<" train_file test_file\n";

	SgdSvm svm;
	//svm.getClassDetail(argv[1]);
	svm.classifier(argv[1], argv[2]);
*/

	// -------------------------------------------------------------
	// ---------------------- Parse Command Line -------------------
	// -------------------------------------------------------------

	std::string data_filename;
	std::string test_filename;
	std::string model_filename;
	std::string experiments_file = "noExperimentsFile";
	double lambda = 1.0;
	int max_iter = 10;
	int exam_per_iter = 1;
	int paral = 0;
	
	//uint num_iter_to_avg = 100;

	// parse command line
	learning::cmd_line cmdline;
	cmdline.info("Pegasos algorithm");
	cmdline.add_master_option("<data-file>", &data_filename);
	cmdline.add("-lambda", "regularization parameter (default = 0.01)", 
				&lambda, 0.001);
	cmdline.add("-iter", "number of iterations (default = 10/lambda)", 
				&max_iter, int(100/lambda));
	cmdline.add("-k", "size of block for stochastic gradient (default = 1)", 
				&exam_per_iter, 1);
	cmdline.add("-modelFile","name of model file (default = noModelFile)", 
				&model_filename,"noModelFile");
	cmdline.add("-testFile","name of test data file (default = noTestFile)", 
				&test_filename,"noTestFile");
	cmdline.add("-paral","param for parallel with openmp (default = 0)", 
				&paral,0);
//   cmdline.add("-experiments","name of experiments spec. file", 
// 	      &experiments_file,"noExperimentsFile");

	int rc = cmdline.parse(argc, argv);
	if (rc < 2) {
		cmdline.print_help();
		return EXIT_FAILURE;
	}
	
	SgdSvm svm;
	svm.classifier(data_filename, test_filename, model_filename, 
				   lambda, max_iter, exam_per_iter, paral);

	return 0;
}
