#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <vector>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>

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

int folder = 0;
bool test = false;
void createCSVFile(const char* dir, std::ofstream& ofs, std::string ext = "png")
{
    DIR *pDIR;
    struct dirent *entry;
    pDIR=opendir(dir);
    if(!pDIR)
	{	
		return;
	}
	
    while((entry = readdir(pDIR)))
    {
		if( strcmp(entry->d_name, ".") != 0 && 
			strcmp(entry->d_name, "..") != 0 )
		{
			std::string path = std::string(dir);
			std::string name = path + "/" + std::string(entry->d_name);
			if(std::string(entry->d_name).find("." + ext, 0) != std::string::npos)
			{
				std::ostringstream oss;
				oss<<folder;
				//name += ";" + oss.str();
				ofs<<name <<std::endl;
			}
			else folder++;
			createCSVFile(name.c_str(), ofs, ext);
		}
    }
    closedir(pDIR);
}


void writeBegining(std::string data, std::string file_name)
{
	std::ifstream ifs(file_name);
	std::string old_data;
	std::string line;
	std::ofstream ofs;
	ofs.open("key.temp", std::ios_base::app);
	
	while(getline(ifs, line))
	{
		ofs<< line + "\n";
	}
	ofs.close();
	ifs.close();
	ifs.open("key.temp");
	ofs.open(file_name.c_str());
	ofs<<data;
	while(getline(ifs, line))
	{
		ofs<< line + "\n";
	}
	ifs.close();
	ofs.close();
	std::system("rm key.temp");
}

unsigned long int getNBLine(std::string file_name)
{
	std::ifstream ifs(file_name.c_str());
	unsigned long int nb_line = 0;
	std::string line;
	while(getline(ifs, line))
		nb_line++;
	return nb_line;
}


int main(int argc, char *argv[])
{
	if(argc < 4)
	{
		
		std::cout<<"usage: " <<argv[0] <<"src_dir csv_file ext ";
		return 1;
		
	}
	

	std::string ext;
	std::ofstream ofs(argv[2]);
	ext = argv[3];
	createCSVFile(argv[1], ofs, ext);
	ofs.close();

	int number_line_firt = 0;
	if(argc >= 5)
		number_line_firt = atoi(argv[4]);
	if(number_line_firt == 1)
	{
		unsigned long int nb_line = getNBLine(argv[2]);
		std::stringstream ss;
		ss<<nb_line;
		std::string s = ss.str() + "\n";
		writeBegining(s, argv[2]);
	}
	
    return 0;
}
