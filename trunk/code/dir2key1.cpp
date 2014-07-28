#include <string>
#include <dirent.h>
#include <vector>
#include <string.h>
#include <iostream>

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

/*
  Cette fonction permet de detecter des keypoints des images
  Elle rappelle la commande sift de l'auteur
  data_train_path est le repertoire de base d'image
  data_key_path est le repertoire de keypoint ou on veut enregistrer
*/
void findKeyPoint(std::string image_path, std::string key_path)
{
    DIR *pDIR;
    struct dirent *entry;

    pDIR=opendir(image_path.c_str());
    if(pDIR){
		while((entry = readdir(pDIR)))
		{ //quand pas encore lire tous les fichiers
			//. et .. est le répertoire actuel et le père
			if( strcmp(entry->d_name, ".") != 0 && 
				strcmp(entry->d_name, "..") != 0 )
			{
				DIR *sDIR;
				struct dirent *s_entry;
				std::string img_path;
				std::string k_path;
				std::string key_name;
				std::string cmd;
				std::vector<std::string> path_elem, name_elem;
				std::string class_dir = image_path + "/" + entry->d_name;
				std::string key_class_dir = key_path + "/" + entry->d_name;
				sDIR=opendir(class_dir.c_str());
				std::cout<<class_dir <<"\n";
				
				if(sDIR){
					std::string cmd_key_class = "mkdir " + key_class_dir;
					std::system(cmd_key_class.c_str());
					while((s_entry = readdir(sDIR)))
					{
						if( strcmp(s_entry->d_name, ".") != 0 && 
							strcmp(s_entry->d_name, "..") != 0 )
						{
							Split(name_elem, s_entry->d_name, '.');
							key_name = name_elem.at(0) + ".key";
							img_path = class_dir + "/" + s_entry->d_name;
							k_path = key_class_dir + "/" + key_name;
							std::cout<<"Extracted image " <<img_path <<" to " <<k_path <<std::endl;
							cmd = "./extractor " + img_path + " " + k_path;
							std::system(cmd.c_str());
						}
		
					}
				
				}
			
			}
	}
	closedir(pDIR);
    }
}

int main(int argc, char* argv[])
{
	if(argc < 3){
        std::cout << "usage: " << argv[0] << " image_dir key_dir" << std::endl;
        exit(1);
    }
	findKeyPoint(argv[1], argv[2]);
}
