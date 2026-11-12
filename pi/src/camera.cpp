#include "camera.hpp"
#include <iostream>

namespace camera {

#ifdef RASPBERRY_PI_CAMERA_PIPELINE
	std::string pipeline = "libcamerasrc ! video/x-raw, width=640, height=480, format=BGR ! appsink";

	cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);

#endif
#ifdef WINDOWS_CAMERA_PIPELINE

	cv::VideoCapture cap(0, cv::CAP_DSHOW);

#endif

	cv::Mat frame;
	
	int init() {
		if (!cap.isOpened()) {
			std::cerr << "Error: Cannot open camera pipeline!" << std::endl;
			return -1;
		}

#ifdef WINDOWS_CAMERA_PIPELINE
		cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
		cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

		cv::namedWindow("Camera Buffer Preview", cv::WINDOW_AUTOSIZE);
#endif

		return 0;
	}
	
	void release() {
		cap.release();

#ifdef WINDOWS_CAMERA_PIPELINE
		cv::destroyAllWindows();
#endif

	}
	
	bool loadFrame() {
		cap >> frame;
		
		return !frame.empty();
	}
}
