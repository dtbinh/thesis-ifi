#include <vector>
#include <string>

/*
  Permet de séparer le nom des fichiers appliqué et leur file
*/
int Split(std::vector<std::string>& vector, std::string str, char separator)
{
    vector.clear();
	std::string::size_type stTemp = str.find(separator);
    while(stTemp != std::string::npos)
    {
		vector.push_back(str.substr(0, stTemp));
		str = str.substr(stTemp + 1);
		stTemp = str.find(separator);
    }
    vector.push_back(str);
    return vector.size();
}
