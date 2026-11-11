#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <opencv2/opencv.hpp>

namespace camera {
	extern std::string pipeline;
		
	extern cv::VideoCapture cap;

	extern cv::Mat frame;

	int init();
	
	void release();
	
	bool loadFrame();
}

#endif
