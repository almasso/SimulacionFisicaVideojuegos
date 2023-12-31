#pragma once
#include "../RenderUtils.hpp"
#include <cmath>
namespace angleutils {
	static float degToRad(float deg) {
		return (deg * physx::PxPi) / 180.0;
	}
	static float radToDeg(float rad) {
		return (rad * 180.0) / physx::PxPi;
	}
}