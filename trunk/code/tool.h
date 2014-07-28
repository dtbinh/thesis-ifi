#ifndef TOOL_H
#define TOOL_H

#include <iostream>
#include <vector>
#include <string>

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class Tool{
public:
	Tool();
	~Tool();

	cv::Mat getImageKeyPoint(cv::Mat image);
	cv::Mat getDescriptor(cv::Mat image);
	std::vector<cv::KeyPoint> getKeyPoints(cv::Mat image);
	
};

#endif
