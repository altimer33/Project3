#include "image_processing.hpp"
#include <opencv2/opencv.hpp>

namespace processing {
	template<typename T>
	BgrValue<T> BgrValue<T>::operator*(T scalar) const {
		return BgrValue{.blue = blue * scalar, .green = green * scalar, .red = red * scalar};
	}

	template<typename T>
	BgrValue<T> BgrValue<T>::operator+(const BgrValue& other) const {
		return BgrValue{.blue = blue + other.blue, .green = green + other.green, .red = red + other.red};
	}

	template<typename T>
	BgrValue<T> BgrValue<T>::operator-=(const BgrValue& other) {
		return BgrValue{.blue = blue - other.blue, .green = green - other.green, .red = red - other.red};
	}

	template<typename T>
	BgrValue<T> BgrValue<T>::operator*=(T scalar) {
		blue *= scalar;
		green *= scalar;
		red *= scalar;
		return *this;
	}

	template<typename T>
	BgrValue<T> BgrValue<T>::operator+=(const BgrValue& other) {
		blue += other.blue;
		green += other.green;
		red += other.red;
		return *this;
	}

	template<typename T>
	BgrValue<T> BgrValue<T>::operator-(const BgrValue& other) const {
		blue -= other.blue;
		green -= other.green;
		red -= other.red;
		return *this;
	}

	template<typename T>
	BgrValue<T> BgrValue<T>::from(T *other) {
		return BgrValue{.blue = other[0],.green = other[1],.red = other[2]};
	}
	
	ImageData::ImageData() : iters{0} {
		for (int x = 0; x < 40; x++) {
			for (int y = 0; y < 30; y++) {
				datas[x][y] = ChunkData(x << 4, y << 4);
			}
		}
	}

	void ImageData::process(cv::Mat &image) {
		iters++;
		for (auto &row : datas) {
			for (auto &chunk : row) {
				chunk.process(image, iters);
			}
		}
	}
	
	ChunkData::ChunkData(uint32_t xMin, uint32_t yMin) : 
		xMin{xMin}, 
		yMin{yMin}
	{}

	ChunkData::ChunkData() {}

	void ChunkData::update(cv::Mat &image, uint16_t iters) {
		for (uint8_t x = 0; x < 16; x++) {
			for (uint8_t y = 0; y < 16; y++) {
				BgrValue<uint16_t> pixMSquare = pixels[x][y].update(iters, BgrValue<uint8_t>::from(reinterpret_cast<uint8_t *>(&image.at<cv::Vec3b>(yMin + y, xMin + x))));
				mSquare += pixMSquare;
			}
		}
	}
}
