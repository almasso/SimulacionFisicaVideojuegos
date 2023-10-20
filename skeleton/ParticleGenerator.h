#pragma once
#include <string>
#include <list>
#include <random>
#include <chrono>
#include <vector>
#include "Particle.h"
#include "BoundingBox.h"

class ParticleGenerator {
protected:
	std::string name;
	Vector3 avrg_pos, avrg_vel;
	int numParticles;
	std::vector<Particle*> models;
	BoundingBox _bb;
	
	inline void setParticle(Particle* model) { models.push_back(model); }
	ParticleGenerator(std::string name, const BoundingBox& bb, const Vector3& genPos, const Vector3& averageVel) : name(name), _bb(bb), avrg_pos(genPos), avrg_vel(averageVel) {}

public:
	inline BoundingBox getBoundingBox() const { return _bb; }
	virtual std::list<Particle*> generateParticles() = 0;
	virtual ~ParticleGenerator();

};

class GaussianParticleGenerator : public ParticleGenerator {
protected:
	Vector3 sigmaVel, sigmaPos;
	std::default_random_engine gen;
	std::normal_distribution<float>* vX;
	std::normal_distribution<float>* vY;
	std::normal_distribution<float>* vZ;
	std::normal_distribution<float>* pX;
	std::normal_distribution<float>* pY;
	std::normal_distribution<float>* pZ;

public:
	GaussianParticleGenerator(std::string name, const BoundingBox& bb, const Point& generationPosition, const Vector3& averagePos, const Vector3& sigmaPos, const Vector3& averageSpeed, const Vector3& sigmaSpeed);
	~GaussianParticleGenerator();
	std::list<Particle*> generateParticles() override;
};

class UniformParticleGenerator : public ParticleGenerator {


};

