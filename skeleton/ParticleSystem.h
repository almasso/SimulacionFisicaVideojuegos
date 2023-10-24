#pragma once
#include <list>
#include <string>
#include "Particle.h"
#include "BoundingBox.h"
#include "ParticleGenerator.h"
#include "checkMemoryLeaks.h"

const float tiempo_borrado = 40.0f;

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
	//void generateFireworkSystem();
};

