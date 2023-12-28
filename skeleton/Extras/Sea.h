#pragma once
#include "../RenderUtils.hpp"
#include "../checkMemoryLeaks.h"
#include "../Utils/BoundingBox.h"

class Sea {
public:
	Sea(BoundingBox* bb, Vector4 col);
	~Sea();
	physx::PxTransform getPos() const { return pose; }
protected:
	physx::PxTransform pose;
	RenderItem* renderItem;
};

