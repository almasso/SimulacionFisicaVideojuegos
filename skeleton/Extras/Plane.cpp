#include "Plane.h"
#include "../checkMemoryLeaks.h"

Plane::Plane(Vector3 pos, Vector3 Dimensions, Vector4 Col) : pose(physx::PxTransform(pos)) {
	physx::PxBoxGeometry planeGeo(Dimensions);
	physx::PxShape* shape = CreateShape(planeGeo);
	renderItem = new RenderItem(shape, &pose, Col);
}

Plane::~Plane() {
	renderItem->release();
}

RigidPlane::RigidPlane(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 pos, Vector3 Dimensions, Vector4 Color) : Plane(pos, Dimensions, Color) {
	renderItem->release();
	physx::PxRigidStatic* suelo = gPhysics->createRigidStatic(pose);
	physx::PxBoxGeometry planeGeo(Dimensions);
	physx::PxShape* shape = CreateShape(planeGeo);
	suelo->attachShape(*shape);
	gScene->addActor(*suelo);
	renderItem = new RenderItem(shape, suelo, Color);
}
