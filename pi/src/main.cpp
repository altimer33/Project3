#include "camera.hpp"

int main() {
	if (camera::init() == -1) {
		return -1;
	}
	
	while (true) {
		if (!camera::loadFrame()) break;
		
		cv::imshow("Camera Buffer Preview", camera::frame);
		if (cv::waitKey(1) == 27) break;
	}
	
	camera::release();
	return 0;
}
