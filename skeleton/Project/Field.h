#pragma once
#include "../Extras/Plane.h"
#include "Verja.h"
#include <vector>
class Field {
private:
	SolidPlane* campo;
	std::vector<Verja*> jaula;
	Vector3* dimensionesCampo = new Vector3(720, 1, 360);
public:
	Field(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 position);
	~Field();
	inline Vector3 getFieldExtremePos() const { return campo->getPos().p - Vector3(dimensionesCampo->x, 0, 0); }
};

