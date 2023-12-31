#pragma once
#include "../RenderUtils.hpp"
#include "../checkMemoryLeaks.h"

class Plane {
public:
	Plane(Vector3 pos, Vector3 Dimensions, Vector4 Color = { 1,0.78,0,1 });
	~Plane();
	virtual physx::PxTransform getPos() const { return pose; }
protected:
	physx::PxTransform pose;
	RenderItem* renderItem;
};

class SolidPlane : public Plane {
private:
	physx::PxRigidActor* plano;
public:
	SolidPlane(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 pos, Vector3 Dimensions, Vector4 Color = { 1, 0.78, 0, 1 });
	~SolidPlane();
	physx::PxTransform getPos() const override { return plano->getGlobalPose(); }
	void setPose(physx::PxTransform p) { static_cast<physx::PxRigidStatic*>(plano)->setGlobalPose(p); }
};

