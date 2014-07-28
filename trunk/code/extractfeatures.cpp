/*
 * =====================================================================================
 *       Filename:  extractfeatures.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  28/05/2014 06:06:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NGUYEN Quoc Khai
 *   Organization:  IFI
 * =====================================================================================
 */

#include "classificator.h"
#include "common.h"

int main(int argc, char **argv){
    if(argc < 3){
        std::cout << "usage: " << argv[0] << " image_name key_name" << std::endl;
        exit(1);
    }

    Classificator classificator;
	classificator.extractDescriptor(argv[1], argv[2]);
    
    return 0;
}
