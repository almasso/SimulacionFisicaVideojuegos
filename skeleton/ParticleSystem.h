#pragma once
#include <list>
#include <string>
#include "Particle.h"
#include "Firework.h"
#include "BoundingBox.h"
#include "ParticleGenerator.h"
#include "checkMemoryLeaks.h"

const float tiempo_borrado = 40.0f;
const float tiempo_fireworks = 5.0f;

class ParticleSystem {
protected:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _particle_generators;
	BoundingBox _bb;
	
public:
	ParticleSystem(const BoundingBox& bb) : _bb(bb) {}
	~ParticleSystem();
	void update(double t);
	inline void addParticleGenerator(ParticleGenerator* pG) { _particle_generators.push_back(pG); };
	ParticleGenerator* getParticleGenerator(std::string name);
	void generateFirework(Vector3 genPos, Vector3 vel, int gen = 4, float damping = 0.998f, Vector4 col = { 0,1,0,1 });
};

