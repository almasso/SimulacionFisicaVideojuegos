#pragma once
#include "../Systems/ParticleSystem.h"

class Martillo {
private:
	SolidParticle* bola, *mango;
	SpringForceGenerator* f1, *f2;
	float offset = 11.7f;
	physx::PxScene* scene;
public:
	Martillo(physx::PxPhysics* gPhysics, physx::PxScene* gScene, ParticleSystem* _pS, Vector3 pos);
	~Martillo() = default;
	void update(double t);
};

