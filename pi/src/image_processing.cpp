#include "image_processing.hpp"
#include <opencv2/opencv.hpp>

namespace processing {
	constexpr uint16_t decayParameter = 8;
	constexpr uint32_t motionCutoff = 30000;

	MotionLocation::MotionLocation(uint32_t x, uint32_t y, bool fire) :
		x{x},
		y{y},
		fire{fire}
	{}

	template<typename T, uint8_t O>
	BgrValue<T, O>::BgrValue() {
		this->red = this->green = this->blue = static_cast<T>(0x10 << O);
	}

	template<typename T, uint8_t O>
	BgrValue<T, O>::BgrValue(T blue, T green, T red) {
		this->blue = blue;
		this->green = green;
		this->red = red;
	}
	
	template<typename T, uint8_t O>
	BgrValue<T, O> BgrValue<T, O>::operator*(T scalar) const {
		return BgrValue(blue * scalar, green * scalar, red * scalar);
	}

	template<typename T, uint8_t O>
	BgrValue<T, O> BgrValue<T, O>::operator/(T scalar) const {
		return BgrValue(
			static_cast<T>(blue / scalar),
			static_cast<T>(green / scalar),
			static_cast<T>(red / scalar)
		);
	}

	template<typename T, uint8_t O>
	BgrValue<T, O> BgrValue<T, O>::operator/(BgrValue other) const {
		return BgrValue(
			static_cast<T>((blue << O) / other.blue),
			static_cast<T>((green << O) / other.green),
			static_cast<T>((red << O) / other.red)
		);
	}

	template<typename T, uint8_t O>
	BgrValue<T, O> BgrValue<T, O>::operator*(BgrValue other) const {
		return BgrValue(
			static_cast<T>((blue * other.blue) >> O),
			static_cast<T>((green * other.green) >> O),
			static_cast<T>((red * other.red) >> O)
		);
	}

	template<typename T, uint8_t O>
	BgrValue<T, O> BgrValue<T, O>::operator+(const BgrValue& other) const {
		return BgrValue(
			blue + other.blue,
			green + other.green,
			red + other.red
		);
	}

	template<typename T, uint8_t O>
	BgrValue<T, O> BgrValue<T, O>::operator-(const BgrValue& other) const {
		return BgrValue(
			static_cast<T>(blue - other.blue),
			static_cast<T>(green - other.green),
			static_cast<T>(red - other.red)
		);
	}

	template<typename T, uint8_t O>
	BgrValue<T, O> BgrValue<T, O>::operator*=(T scalar) {
		blue *= scalar;
		green *= scalar;
		red *= scalar;
		return *this;
	}

	template<typename T, uint8_t O>
	BgrValue<T, O> BgrValue<T, O>::operator+=(const BgrValue& other) {
		blue += other.blue;
		green += other.green;
		red += other.red;
		return *this;
	}

	template<typename T, uint8_t O>
	BgrValue<T, O> BgrValue<T, O>::operator-=(const BgrValue& other) {
		blue -= other.blue;
		green -= other.green;
		red -= other.red;
		return *this;
	}

	template<typename T, uint8_t O>
	T BgrValue<T, O>::sum() const {
		return static_cast<T>(blue + green + red);
	}

	template<typename T>
	BgrValue<T, 0> from(T *other) {
		return BgrValue<T, 0>(other[0], other[1], other[2]);
	}

	ImageProcessor::ImageProcessor() {
		for (int x = 0; x < 40; x++) {
			for (int y = 0; y < 30; y++) {
				datas[x][y] = ChunkData(x << 4, y << 4);
			}
		}
	}

	MotionLocation ImageProcessor::process(cv::Mat &image) {
		uint32_t maxMotion = 0;
		uint32_t maxX = 0;
		uint32_t maxY = 0;
		for (auto &row : datas) {
			for (auto &chunk : row) {
				uint32_t val = chunk.update(image);
				if (val > motionCutoff && val > maxMotion) {
					maxMotion = val;
					maxX = chunk.xMin | 8;
					maxY = chunk.yMin | 8;
				}
			}
		}
		if (maxMotion) {
			cv::rectangle(image, cv::Point(maxX - 4, maxY - 4), cv::Point(maxX + 4, maxY + 4), cv::Scalar(255, 255, 0));
			return {maxX, maxY, true};
		}
		return {0, 0, false};
	}
	
	ChunkData::ChunkData(uint32_t xMin, uint32_t yMin) : 
		xMin{xMin}, 
		yMin{yMin}
	{}

	ChunkData::ChunkData() {}

	uint32_t ChunkData::update(cv::Mat &image) {
		BgrValueI iterationVariation(0, 0, 0);
		for (uint16_t i = 0; i < 0x100; i++) {
			const uint8_t x = i & 0xF;
			const uint8_t y = i >> 4;
			BgrValueI pixVariation = pixels[x][y].update(
				from<uint8_t>(
					reinterpret_cast<uint8_t *>(
						&image.at<cv::Vec3b>(
							static_cast<int>(yMin) + y,
							static_cast<int>(xMin) + x
						)
					)
				)
			);
			iterationVariation += pixVariation * pixVariation;
		}
		BgrValueI diff1 = variation - iterationVariation;
		variation += diff1 / decayParameter;
		BgrValueI diff2 = variation - iterationVariation;
		return std::abs((diff1 * diff2 / variation).sum());
	}

	BgrValueI PixelData::update(BgrValueB pixel) {
		BgrValueF currentPixel = BgrValueF::from(pixel);
		BgrValueF diff1 = mean - currentPixel;
		mean += diff1 / decayParameter;
		BgrValueF diff2 = mean - currentPixel;
		return BgrValueI::from(diff1) * BgrValueI::from(diff2);
	}
}
