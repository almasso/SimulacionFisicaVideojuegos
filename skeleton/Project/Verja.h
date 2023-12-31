#pragma once
#include "../Extras/Plane.h"
class Verja {
private:
	Vector3 posicionInicial;
	SolidPlane* travesanoSuperior = nullptr;
	SolidPlane* travesanoInferior = nullptr;
	SolidPlane* paloIzquierdo = nullptr;
	SolidPlane* paloDerecho = nullptr;
	SolidPlane* mallaCentral = nullptr;
	void rotateRigidActor(SolidPlane* plane, const Vector3& pivot, const physx::PxQuat& rotation);
public:
	Verja(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 position);
	~Verja();
	void rotate(float angle, Vector3 axis);
};

