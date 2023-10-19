#pragma once
#include <list>
#include <string>
#include <random>
#include <chrono>
#include "Particle.h"
#include "BoundingBox.h"
#include "ParticleGenerator.h"

#define tiempo_borrado 10.0f

class ParticleSystem {
protected:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _particle_generators;
	BoundingBox _bb;
	std::default_random_engine gen;
	std::normal_distribution<float>* vX;
	std::normal_distribution<float>* vY;
	std::normal_distribution<float>* vZ;
	Point genPos;
	
public:
	ParticleSystem(const BoundingBox& bB, const Point& genPos, Vector3 avgSpeed, Vector3 sigma);
	void update(double t);
	ParticleGenerator* getParticleGenerator(std::string name);
	//void generateFireworkSystem();
	inline const BoundingBox const getBoundingBox() const { return _bb; }
};

