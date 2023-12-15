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
	plano = gPhysics->createRigidStatic(pose);
	physx::PxBoxGeometry planeGeo(Dimensions);
	physx::PxShape* shape = CreateShape(planeGeo);
	plano->attachShape(*shape);
	gScene->addActor(*plano);
	renderItem = new RenderItem(shape, plano, Color);
}

RigidPlane::~RigidPlane() {
	plano->release();
}
