#pragma once
#include "../Extras/Plane.h"
#include "Verja.h"
#include "../checkMemoryLeaks.h"
#include <vector>
class Field {
private:
	SolidPlane* campo;
	std::vector<Verja*> jaula;
	std::vector<SolidPlane*> vallas;
	Vector3* dimensionesCampo = new Vector3(720, 1, 360);
public:
	Field(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 position);
	~Field();
	inline Vector3 getFieldSouthmostPos() const { return campo->getPos().p - Vector3(dimensionesCampo->x, 0, 0); }
	inline Vector3 getFieldLeftmostPos() const { return campo->getPos().p - Vector3(0, 0, dimensionesCampo->z); }
	inline Vector3 getFieldNorthmostPos() const { return campo->getPos().p + Vector3(dimensionesCampo->x, 0, 0); }
	inline Vector3 getFieldRightmostPos() const { return campo->getPos().p + Vector3(0, 0, dimensionesCampo->z); }
	inline physx::PxActor* getFieldActor() const { return campo->getActor(); }
};

