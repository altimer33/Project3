#ifndef IMAGE_PROCESSING_HPP
#define IMAGE_PROCESSING_HPP

#include "common.hpp"
#include <opencv2/opencv.hpp>

namespace processing {
	template<typename T, uint8_t O>
	struct BgrValue {
		T blue;
		T green;
		T red;

		BgrValue();

		BgrValue(T blue, T green, T red);

		BgrValue operator*(T scalar) const;

		BgrValue operator*(BgrValue other) const;

		BgrValue operator+(const BgrValue& other) const;

		BgrValue operator-(const BgrValue& other) const;

		BgrValue operator/(T scalar) const;

		BgrValue operator/(BgrValue other) const;

		BgrValue operator*=(T scalar);

		BgrValue operator+=(const BgrValue& other);

		BgrValue operator-=(const BgrValue& other);

		T sum() const;

		template<typename U, uint8_t V>
		static BgrValue<T, O> from(BgrValue<U, V> other) {
			if constexpr (O == V) {
				return BgrValue(
					static_cast<T>(other.blue),
					static_cast<T>(other.green),
					static_cast<T>(other.red)
				);
			} else if constexpr (O > V) {
				return BgrValue(
					static_cast<T>(other.blue << (O - V)),
					static_cast<T>(other.green << (O - V)),
					static_cast<T>(other.red << (O - V))
				);
			} else {
				return BgrValue(
					static_cast<T>(other.blue >> (V - O)),
					static_cast<T>(other.green >> (V - O)),
					static_cast<T>(other.red >> (V - O))
				);
			}
		}
	};

	template<typename T>
	BgrValue<T, 0> from(T *other);

	using BgrValueI = BgrValue<int32_t, 4>;
	using BgrValueF = BgrValue<int16_t, 4>;
	using BgrValueB = BgrValue<uint8_t, 0>;

	class PixelData {
		friend class ChunkData;
		
		BgrValueF mean;

	public:
		BgrValueI update(BgrValueB pixel);
	};

	class ChunkData {
		friend class ImageProcessor;

		uint32_t xMin;
		uint32_t yMin;

		BgrValueI variation = BgrValueI(0, 0, 0);
		PixelData pixels[16][16];

		ChunkData();

	public:
		ChunkData(uint32_t xMin, uint32_t yMin);

		uint32_t update(cv::Mat &image);
	};

	class ImageProcessor {
		ChunkData datas[40][30];

	public:
		ImageProcessor();

		MotionLocation process(cv::Mat &image);
	};
}

#endif
