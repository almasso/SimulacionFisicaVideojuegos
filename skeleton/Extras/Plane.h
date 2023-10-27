#pragma once
#include "../RenderUtils.hpp"
#include "../checkMemoryLeaks.h"

class Plane {
public:
	Plane(Vector3 pos, Vector3 Dimensions, Vector4 Color = { 1,0.78,0,1 });
	~Plane();
private:
	physx::PxTransform pose;
	RenderItem* renderItem;
};

