//=============================================================================
// File Name: cmd_line.cc
// Written by: Joseph Keshet (15.02.04)
//=============================================================================

//*****************************************************************************
// Included Files
//*****************************************************************************
#include <iostream>
#include "cmd_line.h"
#include "cmd_option.h"

using namespace std;
using namespace learning;

//-----------------------------------------------------------------------------

void cmd_line::add(const string &tag, const string &helper, int *value,
				   const int default_value) 
{
	cmd_option *cmd_option = 
		new type_cmd_option<int>(tag, helper, value, default_value);
	options_vector.push_back(cmd_option);
}

//-----------------------------------------------------------------------------

void cmd_line::add(const string &tag, const string &helper,
				   unsigned int *value, const int default_value) 
{
	cmd_option *cmd_option = 
		new type_cmd_option<unsigned int>(tag, helper, value, default_value);
	options_vector.push_back(cmd_option);
}

//-----------------------------------------------------------------------------

void cmd_line::add(const string &tag, const string &helper, double *value,
				   const double default_value) 
{
	cmd_option *cmd_option = 
		new type_cmd_option<double>(tag, helper, value, default_value);
	options_vector.push_back(cmd_option);
}

//-----------------------------------------------------------------------------

void cmd_line::add(const string &tag, const string &helper, bool *value,
				   const bool default_value) 
{
	cmd_option *cmd_option = 
		new type_cmd_option<bool>(tag, helper, value, default_value);
	options_vector.push_back(cmd_option);
}

//-----------------------------------------------------------------------------

void cmd_line::add(const string &tag, const string &helper, string *value,
				   const string &default_value) 
{
	cmd_option *cmd_option = 
		new type_cmd_option<string>(tag, helper, value, default_value);
	options_vector.push_back(cmd_option);
}

//-----------------------------------------------------------------------------

void cmd_line::add(cmd_option *cmd_option) 
{
    options_vector.push_back(cmd_option);
}

//-----------------------------------------------------------------------------

void cmd_line::add_master_option(const std::string &helper, 
								 std::string *master_option)
{
	cmd_option *cmd_option = 
		new type_cmd_option<string>("", helper, master_option, "");
	master_options_vector.push_back(cmd_option);
}

//-----------------------------------------------------------------------------

int cmd_line::parse(int argc, char **argv) 
{
	int num_arg = 0;
	// Set program name as the first argument
	program_name = argv[0];
	if (argc == 1) {
		return num_arg;
	}
  
	// Run over all arguments of the command
	bool processing_options = true;
	int i = 1;
	unsigned int master_count = 0;
	while ( i < argc ) {
		// Check if this is an command options or master argument
		if (processing_options && argv[i][0] == '-') {
      
			// Run over all command options
			vector<cmd_option*>::iterator I;
			for (I = options_vector.begin(); I != options_vector.end(); I++) {
				cmd_option *option = *I;
				string &tag = option->tag();
				// check if the current argument is the same as the option's tag
				if (tag.compare(argv[i]) == 0) {
					// check if the option needs a value after the tag
					if (option->needs_value()) {
						// check if such argument exists
						if (i+1 < argc) {
							// set its value
							option->set_value(argv[i+1]);
						}
						else {
							cerr << program_name << ": Expecting parameter value after "
								 << argv[i] << endl;
							return -1;
						}
						// increase the counts over the arguments by 2
						i += 2;
						break;
					} else {
						// set a flag with no extra value
						option->set_value(argv[i]);
						i++;
						break;
					}
				}
			}
			// If we concluded the loop over all options and didn;t find any mate for
			// the option's tag, then argv[i] is not a valid argument
			if (I == options_vector.end()) {
				cerr << program_name << ": " << argv[i] 
					 << " is not a valid argument." << endl;
				return -1;
			}
		} else {
			// the rest of the arguments are master options
			processing_options = false;
			if (master_count < master_options_vector.size()) {
				master_options_vector[master_count]->set_value(argv[i]);
				master_count++;
				i++;
			}
			else {
				cerr << program_name << ": too many master arguments (" << argv[i] 
					 << ")." << endl;
				return -1;
			}
		}
		// increase number of argument parsed by one
		num_arg++;
	}
	// return number of arguments passed
	return num_arg;
}

//-----------------------------------------------------------------------------

void cmd_line::print_help()
{
	vector<cmd_option*>::iterator I;

	cerr << endl;
	cerr << "USAGE: " << program_name;
	if (options_vector.size() > 0)
		cerr << " [options]";
	for (I = master_options_vector.begin(); I != master_options_vector.end(); 
		 I++) {
		cmd_option *option = *I;
		cerr << " " << option->helper();
	}
	cerr << endl << endl;
	cerr << info_ << endl;
	if (options_vector.size() > 0) {
		cerr << endl;
		cerr << "OPTIONS:" << endl;
	}
	for (I = options_vector.begin(); I != options_vector.end(); I++) {
		cmd_option *option = *I;
		cerr << " " << option->tag() << " " << option->helper() << endl;
	}
	cerr << endl;  
 
}

cmd_line::~cmd_line()
{
	vector<cmd_option*>::iterator I;
	for (I = options_vector.begin(); I != options_vector.end(); I++) {
		delete (*I);
	}
}

//-----------------------------------------------------------------------------
