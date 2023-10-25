#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"
#include <list>
class Firework : public Particle {
private:
	int gen;
public:
	Firework(int gen) : Particle(Particle::Particle_Type::FIREWORK), gen(gen) {}
	std::list<Particle*> explode();
};

