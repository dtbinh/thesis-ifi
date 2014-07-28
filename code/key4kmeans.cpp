#include <string>
#include <dirent.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

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


/*
  Cette fonction permet de detecter des keypoints des images
  Elle rappelle la commande sift de l'auteur
  data_train_path est le repertoire de base d'image
  data_key_path est le repertoire de keypoint ou on veut enregistrer
*/
unsigned long int nb_feature = 0;
uint demension = 0;
void key4Kmeans(std::string key_dir, std::string kmeans_file)
{
    DIR *pDIR;
    struct dirent *entry;
	std::ofstream ofs;
	ofs.open(kmeans_file.c_str(), std::ios_base::app);

    pDIR=opendir(key_dir.c_str());
    if(pDIR){
		while((entry = readdir(pDIR)))
		{ //quand pas encore lire tous les fichiers
			//. et .. est le répertoire actuel et le père
			if( strcmp(entry->d_name, ".") != 0 && 
				strcmp(entry->d_name, "..") != 0 )
			{
				std::string sub_key_dir = key_dir + "/" + entry->d_name;
				DIR *sDIR;
				sDIR = opendir(sub_key_dir.c_str());
				if(sDIR)
				{
					key4Kmeans(sub_key_dir, kmeans_file);
				}
				else
				{
					std::ifstream ifs(sub_key_dir.c_str());
					uint dem, N;
					ifs>>dem >>N;
					demension = dem;
					nb_feature += N;
					std::string line;
					std::getline(ifs, line);
					for(uint i = 0; i < N; i++)
					{
						std::getline(ifs, line);
						ofs<<line <<"\n";
					}
					
				}
			}
		}
		closedir(pDIR);
    }
}

void key4KmeansCSV(std::string csvkey_file, std::string kmeans_file)
{
	std::ofstream ofs;
	ofs.open(kmeans_file.c_str(), std::ios_base::app);

	std::ifstream ifs(csvkey_file.c_str());
	unsigned long int nb_line;
	std::string line;
	ifs>>nb_line;
	getline(ifs, line);
	for(unsigned long int i = 0; i < nb_line; i++)
	{
		std::getline(ifs, line);
		std::ifstream sifs(line.c_str());
		unsigned long int N;
		uint dem;
		sifs>>dem >>N;
		demension = dem;
		nb_feature += N;
		std::string sline;
		std::getline(sifs, sline);
		for(uint i = 0; i < N; i++)
		{
			std::getline(sifs, sline);
			ofs<<sline <<"\n";
		}
	}
}


int main(int argc, char* argv[])
{
	if(argc < 3){
        std::cout << "usage: " << argv[0] << " csvkey_file kmeans_data_file" << std::endl;
        return 1;
    }
    key4KmeansCSV(argv[1], argv[2]);
	std::stringstream ss;
	ss<<nb_feature;
	std::string s = ss.str();
	ss.str(std::string());
	ss<<demension;
	s += " " + ss.str() + "\n";
	writeBegining(s, argv[2]);
	return 0;
}
