#include "Martillo.h"
#include "../Utils/ColorUtils.h"
#include "../checkMemoryLeaks.h"
#include "../Generators/ForceGenerator.h"

Martillo::Martillo(physx::PxPhysics* gPhysics, physx::PxScene* gScene, ParticleSystem* _pS, Vector3 pos) {
	bola = new SolidParticle(gPhysics, gScene, Particle::Particle_Type::SOLID, Particle::Particle_Shape::SPHERE, 1/7.0, pos + Vector3(offset, 0, 0), Vector3(0,0,0), 0.998, colorutils::hexToVec4(0xC0C0C0));
	mango = new SolidParticle(gPhysics, gScene, Particle::Particle_Type::SOLID, Particle::Particle_Shape::CUBE, 3.0f, pos, Vector3(0,0,0), 0.998, colorutils::hexToVec4(0xFF4500));
	
	f1 = new SpringForceGenerator(1, offset, mango);
	f2 = new SpringForceGenerator(1, offset, bola);

	_pS->addSpring(bola, mango, f1, f2);
}