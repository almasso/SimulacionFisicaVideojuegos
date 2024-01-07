#pragma once
#include "../Systems/ParticleSystem.h"

class Martillo {
private:
	SolidParticle* bola, *mango;
	SpringForceGenerator* f1, *f2;
	float offset = 11.7f;
public:
	Martillo(physx::PxPhysics* gPhysics, physx::PxScene* gScene, ParticleSystem* _pS, Vector3 pos);
	~Martillo() = default;
};

