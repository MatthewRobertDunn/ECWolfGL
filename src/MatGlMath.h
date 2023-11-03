#pragma once
#include <wl_def.h>
#include <cmath>
namespace MatGl
{
	struct Vec2dInt {
		Vec2dInt(int x, int y) {
			this->x = x;
			this->y = y;
		}
		int x;
		int y;

		bool const operator==(const Vec2dInt& o) const {
			return x == o.x && y == o.y;
		}

		bool const operator<(const Vec2dInt& o) const {
			return x < o.x || (x == o.x && y < o.y);
		}
	};

	inline float FixedToFloat(fixed x) {

		float result = (float)(x >> 16) + (float)(x & 0xFFFF) / 0xFFFF;
		return result;
	}

	inline float AngleToFloat(angle_t x) {
		return ((float)x / (float)0xFFFFFFFF) * 2 * PI;
	}

	inline fixed FloatToFixed(float x) {

		float whole;
		float fractional = std::modf(x, &whole);

		whole = whole * 0x10000; //Shift whole up to the top 16 bits position
		fractional = fractional * 0xFFFF;

		fixed result = ((fixed)whole & 0xFFFF0000) + ((fixed)fractional & 0xFFFF);
		return result;
	}
}
