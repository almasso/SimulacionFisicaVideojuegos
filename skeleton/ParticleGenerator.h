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
	float avrg_lifeTime;
	int numParticles;
	std::vector<Particle*> models;
	BoundingBox _bb;
	
	inline void setParticle(Particle* model) { models.push_back(model); }
	ParticleGenerator(std::string name, const BoundingBox& bb, const Vector3& genPos, const Vector3& averageVel, float averageLifeTime) : name(name), _bb(bb), avrg_pos(genPos), avrg_vel(averageVel), avrg_lifeTime(averageLifeTime) {}

public:
	inline std::string getName() const { return name; }
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
	std::normal_distribution<float>* t;

public:
	GaussianParticleGenerator(std::string name, const BoundingBox& bb, const Point& generationPosition, const Vector3& sigmaPos, const Vector3& averageSpeed, const Vector3& sigmaSpeed, float averageLifeTime, float sigmaLifeTime);
	~GaussianParticleGenerator();
	std::list<Particle*> generateParticles() override;
};

class UniformParticleGenerator : public ParticleGenerator {


};

