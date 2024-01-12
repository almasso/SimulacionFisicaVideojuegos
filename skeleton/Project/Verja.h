#pragma once
#include "../Extras/Plane.h"
#include "../checkMemoryLeaks.h"
class Verja {
private:
	Vector3 posicionInicial;
	SolidPlane* travesanoSuperior = nullptr;
	SolidPlane* travesanoInferior = nullptr;
	SolidPlane* paloIzquierdo = nullptr;
	SolidPlane* paloDerecho = nullptr;
	SolidPlane* mallaCentral = nullptr;
public:
	Verja(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 position);
	~Verja();
	void rotate(float angle, Vector3 axis);
};

