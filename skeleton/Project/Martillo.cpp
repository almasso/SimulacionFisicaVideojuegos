#include "Martillo.h"
#include "../Utils/ColorUtils.h"
#include "../checkMemoryLeaks.h"
#include "../Generators/ForceGenerator.h"
#include "../Utils/AngleUtils.h"

Martillo::Martillo(physx::PxPhysics* gPhysics, physx::PxScene* gScene, ParticleSystem* _pS, Vector3 pos) : scene(gScene) {
	bola = new SolidParticle(gPhysics, gScene, Particle::Particle_Type::SOLID, Particle::Particle_Shape::SPHERE, 1/7.0, pos + Vector3(offset, 0, 0), Vector3(0,0,0), 0.998, colorutils::hexToVec4(0xC0C0C0));
	mango = new SolidParticle(gPhysics, gScene, Particle::Particle_Type::SOLID, Particle::Particle_Shape::CUBE, 3.0f, pos, Vector3(0,0,0), 0.998, colorutils::hexToVec4(0xFF4500));
	
	mango->getRigidActor()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

	f1 = new SpringForceGenerator(0.01, offset, mango);
	f2 = new SpringForceGenerator(0.01, offset, bola);

	apoyoBola = new SolidPlane(gPhysics, gScene, pos + Vector3(offset, -1, 0), Vector3(1, 1, 1), colorutils::rgbaToVec4(255, 255, 255, 0));


	_pS->addSpring(bola, mango, f1, f2);
	posicionInicial = pos;

	
}

Martillo::~Martillo() {
	if(apoyoBola) delete apoyoBola;
}

void Martillo::move(float rot) {
	if (rot >= 10.0f) rot = 9.9f;
	velTotal = rot;
	physx::PxQuat rotacion(angleutils::degToRad(rot), Vector3(0,1,0));
	angleutils::rotateRigidActor(apoyoBola, posicionInicial - Vector3(5,0,0), rotacion);
	angleutils::rotateRigidActor(mango, posicionInicial - Vector3(5, 0, 0), rotacion);
	angleutils::rotateRigidActor(bola, posicionInicial - Vector3(5, 0, 0), rotacion);
	if (apoyoBola->getPos().p.y <= posicionInicial.y + 3.0f) {
		apoyoBola->setPose(physx::PxTransform(apoyoBola->getPos().p + Vector3(0, 0.01, 0)));
	}
}

void Martillo::lanzar() {
	mango->getRigidActor()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, false);
	apoyoBola->setPose(physx::PxTransform(Vector3(999999999, 999999999, 99999999)));
}

void Martillo::update(double t) {
}