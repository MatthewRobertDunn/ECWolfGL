#pragma once
#include <wl_def.h>
#include <cmath>
#include "glm/glm.hpp"
namespace MatGl
{
	// Custom comparison function for glm::ivec2
	struct IVec2Compare {
		inline bool operator()(const glm::ivec2& lhs, const glm::ivec2& rhs) const {
			return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
		};
	};

	struct IVec3Compare
	{
		inline bool operator()(const glm::ivec3& lhs, const glm::ivec3& rhs) const
		{
			return lhs.x < rhs.x || (lhs.x == rhs.x && (lhs.y < rhs.y || (lhs.y == rhs.y && lhs.z < rhs.z)));
		}
	};

	inline float FixedToFloat(fixed x) {

		return (float)x / (float)0xFFFF;
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
