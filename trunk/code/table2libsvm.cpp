#include <string>
#include <iostream>
#include <fstream>
#include <vector>

/*
  Permet de séparer le nom des fichiers appliqué et leur file 
*/
int Split(std::vector<std::string>& vecteur, std::string chaine, char separateur)
{
    vecteur.clear();
    std::string::size_type stTemp = chaine.find(separateur);
    while(stTemp != std::string::npos)
    {
	vecteur.push_back(chaine.substr(0, stTemp));
	chaine = chaine.substr(stTemp + 1);
	stTemp = chaine.find(separateur);
    }
    vecteur.push_back(chaine);
    return vecteur.size();
}

std::vector<uint> getLabels(std::string csvkey_file)
{
	std::vector<uint> labels;
	std::ifstream ifs(csvkey_file.c_str());
	unsigned long int nb_row;
	ifs>>nb_row;
	std::string line;
	getline(ifs, line);
	std::string old_line = line;
	uint label = 0;
	for(unsigned long int i = 0; i < nb_row; i++)
	{
		getline(ifs, line);
		std::vector<std::string> v1, v2;
		Split(v1, old_line, '/');
		v1.pop_back();
		Split(v2, line, '/');
		v2.pop_back();
		
		if(v1 != v2)
		{
			label++;
			old_line = line;
		}
		labels.push_back(label);
	}
	return labels;
}


void table2LibSVM(std::string csvkey_file, std::string table_file, std::string libsvm_file)
{
	std::vector<uint> labels = getLabels(csvkey_file);
	std::ifstream ifs(table_file.c_str());
	std::ofstream ofs(libsvm_file.c_str());
	unsigned long int nb_row, nb_col;
	ifs>>nb_row >>nb_col;
	if(nb_row != labels.size())
		return;
	int value;
	
	for(unsigned long int i = 0; i < nb_row; i++)
	{
		ofs<<labels[i] <<" ";
		for(uint j = 0; j < nb_col; j++)
		{
			ifs>>value;
			if(value != 0)
			{				
				ofs<<j + 1 <<":" <<value;
				if(j < nb_col - 1) 
					ofs<<" ";
			}
			if(j == nb_col - 1)
				ofs<<"\n";
		}
	}
}

int main(int argc, char* argv[])
{
	if(argc < 4)
	{	
		std::cout<<"Usage: " <<argv[0] <<" csvkey_file table_file libsvm_file";
		return 1;
	}
	table2LibSVM(argv[1], argv[2], argv[3]);
	return 0;
	
}

