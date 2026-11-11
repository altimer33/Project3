#ifndef IMAGE_PROCESSING_HPP
#define IMAGE_PROCESSING_HPP

#include <opencv2/opencv.hpp>

namespace processing {
	template<typename T>
	struct BgrValue {
		T blue;
		T green;
		T red;

		BgrValue operator*(T scalar) const;

		BgrValue operator+(const BgrValue& other) const;

		BgrValue operator-(const BgrValue& other) const;

		BgrValue operator*=(T scalar);

		BgrValue operator+=(const BgrValue& other);

		BgrValue operator-=(const BgrValue& other);

		static BgrValue from(T *other);
	};

	class PixelData {
		friend class ChunkData;
		
		BgrValue<uint16_t> mean;

	public:
		BgrValue<uint16_t> update(uint16_t iters, BgrValue<uint8_t> pixel);
		void clear();
	};

	class ChunkData {
		friend class ImageData;

		uint32_t xMin;
		uint32_t yMin;

		BgrValue<uint16_t> mSquare = {.blue = 0, .green = 0,.red = 0};
		PixelData pixels[16][16];

		ChunkData();

	public:
		ChunkData(uint32_t xMin, uint32_t yMin);

		void update(cv::Mat &image, uint16_t iters);
		void clear();

		uint32_t process(const cv::Mat &mat, uint16_t iters);
	};

	class ImageData {
		uint16_t iters;
		ChunkData datas[40][30];

	public:
		ImageData();

		void process(cv::Mat &image);
	};
}

#endif
