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
void findKeyPoint(std::string src_dir, std::string des_dir)
{
    DIR *pDIR;
    struct dirent *entry;

    pDIR=opendir(src_dir.c_str());
    if(pDIR){
		std::string cmd = "mkdir -p " + des_dir;
		std::system(cmd.c_str());
		while((entry = readdir(pDIR)))
		{ //quand pas encore lire tous les fichiers
			//. et .. est le répertoire actuel et le père
			if( strcmp(entry->d_name, ".") != 0 && 
				strcmp(entry->d_name, "..") != 0 )
			{
				std::string sub_src_dir = src_dir + "/" + entry->d_name;
				std::string sub_des_dir = des_dir + "/" + entry->d_name;
				DIR *sDIR;
				sDIR = opendir(sub_src_dir.c_str());
				if(sDIR)
				{
					cmd = "mkdir -p " + sub_des_dir;
					std::system(cmd.c_str());
					findKeyPoint(sub_src_dir, sub_des_dir);
				}
				else
				{
					std::vector<std::string> name_elem;
					Split(name_elem, sub_des_dir, '.');
					std::string key_name = name_elem.at(0) + ".key";
					Split(name_elem, sub_src_dir, '.');
					std::string img_name = name_elem.at(0) + ".pgm";
					cmd = "convert " + sub_src_dir + " " + img_name;
					std::system(cmd.c_str());
					std::cout<<img_name <<" to " <<key_name <<"\n";
					//std::string cmd = "../code_ref/siftDemoV4/sift<" + 
					//	img_name + "> " + key_name;
					std::string cmd = "./extractor " + img_name + " " + key_name;
					std::system(cmd.c_str());
					cmd = "rm -fr " + img_name;
					std::system(cmd.c_str());
				}
				
			}
		}
		closedir(pDIR);
    } else {
		return;
	}
}

int main(int argc, char* argv[])
{
	if(argc < 3){
        std::cout << "usage: " << argv[0] << " image_dir key_dir" << std::endl;
        return 1;
    }
	findKeyPoint(argv[1], argv[2]);
	return 0;
	
}
