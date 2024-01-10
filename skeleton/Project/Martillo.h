#pragma once
#include "../Systems/ParticleSystem.h"
#include "../Extras/Plane.h"

class Martillo {
private:
	SolidParticle* bola, *mango;
	SolidPlane* apoyoBola;
	SpringForceGenerator* f1, *f2;
	float offset = 11.7f;
	physx::PxScene* scene;
	Vector3 posicionInicial;
	float velTotal;
public:
	Martillo(physx::PxPhysics* gPhysics, physx::PxScene* gScene, ParticleSystem* _pS, Vector3 pos);
	~Martillo();
	void update(double t);
	void move(float rot);
	void lanzar();
	Vector3 getPos() const { return mango->getPosition().p; }
	float getVel() const { return velTotal; }
};

