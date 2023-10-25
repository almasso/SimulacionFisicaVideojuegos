#pragma once
#include "Particle.h"
#include "ParticleGenerator.h"
#include <list>

class Firework : public Particle {
private:
	int gen;
public:
	Firework(int gen, Vector3 Pos, Vector3 Vel, float damping = 0.998, Vector4 Col = { 1,0,0,1 }, bool affectedByGravity = false) : Particle(Particle_Type::FIREWORK, Pos, Vel, damping, Col, true), gen(gen) {}
	Firework(int gen, float damping = 0.998f, Vector4 col = {0,1,0,1}) : Particle(Particle::Particle_Type::FIREWORK, damping, col), gen(gen) {}
	
	inline int getGen() const { return gen; }
	std::list<Particle*> explode();
};

