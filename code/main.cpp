#include <iostream>
#include "io.h"
#include "tool.h"


int main(int argc, char* argv[])
{

	std::vector<std::string> list_name;
	std::vector<cv::Mat> list_image;
	list_name.push_back("images/image1.png");
	
	


	IO io;
	list_image = io.getImages(list_name);
	cv::imwrite("images/img1_out.png", list_image[0]);

	Tool tool;
	cv::Mat image_keypoint;
	image_keypoint = tool.getImageKeyPoint(list_image[0]);
	cv::imwrite("images/img1_keypoint.png", image_keypoint);
	cv::Mat img_descriptor = tool.getDescriptor(list_image[0]);
	cv::imwrite("images/img1_descriptor.png", img_descriptor);
	return 0;
}
