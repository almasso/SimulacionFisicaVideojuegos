#pragma once
#include "ParticleSystem.h"
#include <string>
#include <list>

class ParticleGenerator {
protected:
	std::string name;
	Vector3 avrg_pos, avrg_vel;
	double generationProbability;
	int numParticles;
	Particle* model = nullptr;

public:

	inline void setParticle(Particle* model) { this->model = model; }
	virtual std::list<Particle*> generateParticles() = 0;
	virtual ~ParticleGenerator();

};

class GaussianParticleGenerator : public ParticleGenerator {
protected:
	Vector3 std_dev_pos, std_dev_vel;
	double std_dev_t;

public:
	virtual std::list<Particle*> generateParticles() override;
};

class UniformParticleGenerator : public ParticleGenerator {


};

