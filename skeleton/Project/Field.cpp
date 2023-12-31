#include "Field.h"
#include "../Utils/ColorUtils.h"

Field::Field(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 position) : jaula(8, nullptr) {
	campo = new SolidPlane(gPhysics, gScene, position, *dimensionesCampo, colorutils::hexToVec4(0x009A17));
	jaula[0] = new Verja(gPhysics, gScene, getFieldExtremePos() + Vector3(15, 2, -16));
	jaula[0]->rotate(70, Vector3(0, 1, 0));
	jaula[1] = new Verja(gPhysics, gScene, getFieldExtremePos() + Vector3(15, 2, 16));
	jaula[1]->rotate(-70, Vector3(0, 1, 0));
	jaula[2] = new Verja(gPhysics, gScene, getFieldExtremePos() + Vector3(37, 2, -38));
	jaula[2]->rotate(20, Vector3(0, 1, 0));
	jaula[3] = new Verja(gPhysics, gScene, getFieldExtremePos() + Vector3(37, 2, 38));
	jaula[3]->rotate(-20, Vector3(0, 1, 0));
	jaula[4] = new Verja(gPhysics, gScene, getFieldExtremePos() + Vector3(69, 2, -44));
	jaula[5] = new Verja(gPhysics, gScene, getFieldExtremePos() + Vector3(69, 2, 44));
	jaula[6] = new Verja(gPhysics, gScene, getFieldExtremePos() + Vector3(100, 2, -36));
	jaula[6]->rotate(-30, Vector3(0, 1, 0));
	jaula[7] = new Verja(gPhysics, gScene, getFieldExtremePos() + Vector3(100, 2, 36));
	jaula[7]->rotate(30, Vector3(0, 1, 0));
}

Field::~Field() {
	for (auto p : jaula) if(p) delete p;
	delete campo;
	delete dimensionesCampo;
}