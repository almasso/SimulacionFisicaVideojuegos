#pragma once
#include <list>
#include <string>
#include "Particle.h"
#include "BoundingBox.h"
class ParticleSystem {
protected:
	std::list<Particle*> _particles;
	//std::list<ParticleGenerator*> _particle_generators;
	BoundingBox _bb;
	
public:
	ParticleSystem(const BoundingBox& bB);
	void update(double t);
	//ParticleGenerator* getParticleGenerator(std::string name);
	void generateFireworkSystem();
	inline const BoundingBox const getBoundingBox() const { return _bb; }
};

