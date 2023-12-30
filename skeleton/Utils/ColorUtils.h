#pragma once
#include "../checkMemoryLeaks.h"
#include "../RenderUtils.hpp"
namespace colorutils {
	inline static Vector4 rgbaToVec4(int r, int g, int b, float a) {
		return Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a);
	}
	inline static Vector4 hexToVec4(int hexval) {
		return Vector4(((hexval >> 16) & 0xFF) / 255.0f, ((hexval >> 8) & 0xFF) / 255.0f, (hexval & 0xFF) / 255.0f, 1.0f);
	}
} 