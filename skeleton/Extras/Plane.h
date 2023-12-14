#pragma once
#include "../RenderUtils.hpp"
#include "../checkMemoryLeaks.h"

class Plane {
public:
	Plane(Vector3 pos, Vector3 Dimensions, Vector4 Color = { 1,0.78,0,1 });
	virtual ~Plane();
	physx::PxTransform getPos() const { return pose; }
protected:
	physx::PxTransform pose;
	RenderItem* renderItem;
};

class RigidPlane : public Plane {
public:
	RigidPlane(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 pos, Vector3 Dimensions, Vector4 Color = { 1, 0.78, 0, 1 });
	~RigidPlane() = default;
};

