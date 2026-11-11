#include "camera.hpp"
#include <iostream>

namespace camera {
	std::string pipeline = "libcamerasrc ! video/x-raw, width=640, height=480, format=BGR ! appsink";
	
	cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
	
	cv::Mat frame;
	
	int init() {
		if (!cap.isOpened()) {
			std::cerr << "Error: Cannot open camera pipeline!" << std::endl;
			return -1;
		}
		return 0;
	}
	
	void release() {
		cap.release();
	}
	
	bool loadFrame() {
		cap >> frame;
		
		return !frame.empty();
	}
}
