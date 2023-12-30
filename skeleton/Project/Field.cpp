#include "Field.h"
#include "../Utils/ColorUtils.h"

Field::Field(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 position) {
	campo = new SolidPlane(gPhysics, gScene, position, *dimensionesCampo, colorutils::hexToVec4(0x009A17));
}

Field::~Field() {
	delete campo;
	delete dimensionesCampo;
}