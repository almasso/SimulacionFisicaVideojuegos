#include "Verja.h"
#include "../Utils/ColorUtils.h"
#include "../Utils/AngleUtils.h"
Verja::Verja(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Vector3 position) : posicionInicial(position) {
	travesanoInferior = new SolidPlane(gPhysics, gScene, position, Vector3(15,0.9,0.9), colorutils::hexToVec4(0xcc1758));
	travesanoSuperior = new SolidPlane(gPhysics, gScene, position + Vector3(0, 60, 0), Vector3(15,0.9,0.9), colorutils::hexToVec4(0xcc1758));
	paloIzquierdo = new SolidPlane(gPhysics, gScene, position + Vector3(-15.9, 30, 0), Vector3(0.9, 30.9, 0.9), colorutils::hexToVec4(0xcc1758));
	paloDerecho = new SolidPlane(gPhysics, gScene, position + Vector3(15.9, 30, 0), Vector3(0.9, 30.9, 0.9), colorutils::hexToVec4(0xcc1758));
	mallaCentral = new SolidPlane(gPhysics, gScene, position + Vector3(0, 30, 0), Vector3(15, 30, 0.45), colorutils::rgbaToVec4(10,10,10,0.3f));
}

Verja::~Verja() {
	delete travesanoInferior;
	delete travesanoSuperior;
	delete paloDerecho;
	delete paloIzquierdo;
	delete mallaCentral;
}

void Verja::rotate(float angle, Vector3 axis) {
	physx::PxQuat rotation(angleutils::degToRad(angle), axis);
	rotateRigidActor(travesanoSuperior, posicionInicial, rotation);
	rotateRigidActor(travesanoInferior, posicionInicial, rotation);
	Vector3 paloOffset(0.0f, 30.0f, 0.0f);
	rotateRigidActor(paloDerecho, posicionInicial + paloOffset, rotation);
	rotateRigidActor(paloIzquierdo, posicionInicial - paloOffset, rotation);
	rotateRigidActor(mallaCentral, posicionInicial, rotation);
}

void Verja::rotateRigidActor(SolidPlane* plane, const Vector3& pivot, const physx::PxQuat& rotation) {
	Vector3 newPosition = rotation.rotate(plane->getPos().p - pivot) + pivot;
	physx::PxQuat newOrientation = rotation * plane->getPos().q;
	plane->setPose(physx::PxTransform(newPosition, newOrientation));
}