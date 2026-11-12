#include <opencv2/opencv.hpp>
#include "camera.hpp"
#include "image_processing.hpp"

int main() {
	std::cout << "Hello World!" << std::endl;

	if (camera::init() == -1) {
		return -1;
	}

#ifdef WINDOWS_CAMERA_PIPELINE
	processing::initSettingsWindow();
#endif

	processing::ImageProcessor processor = processing::ImageProcessor();
	
	while (true) {
		if (!camera::loadFrame()) break;

		auto [x, y, fire] = processor.process(camera::frame);
		
		cv::imshow("Camera Buffer Preview", camera::frame);
		if (cv::waitKey(10) == 27) break;
	}
	
	camera::release();
	return 0;
}
