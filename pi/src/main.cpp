#include <opencv2/opencv.hpp>
#include "camera.hpp"
#include "image_processing.hpp"

int main() {
	std::cout << "Hello World!" << std::endl;

	if (camera::init() == -1) {
		return -1;
	}

	processing::ImageProcessor processor = processing::ImageProcessor();
	
	while (true) {
		if (!camera::loadFrame()) break;

		auto [x, y, fire] = processor.process(camera::frame);

		if (fire) {
			std::cout << "Fire at " << (int) x << ", " << (int) y << std::endl;
		}
		
		cv::imshow("Camera Buffer Preview", camera::frame);
		if (cv::waitKey(10) == 27) break;
	}
	
	camera::release();
	return 0;
}
