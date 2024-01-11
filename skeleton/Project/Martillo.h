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
	ParticleSystem* _pS;
	bool linkedToPS = false;
public:
	Martillo(physx::PxPhysics* gPhysics, physx::PxScene* gScene, ParticleSystem* _pS, Vector3 pos);
	~Martillo();
	void move(float rot);
	void lanzar();
	inline Vector3 getPos() const { return mango->getPosition().p; }
	inline float getVel() const { return velTotal; }
	inline physx::PxActor* getBallActor() const { return bola->getRigidActor(); }
	inline float getDistance() const { return bola->getPosition().p.x - posicionInicial.x; }
	inline Vector3 getBallPos() const { return bola->getPosition().p; }
	inline void unlinkFromPS() { linkedToPS = false; }
};

