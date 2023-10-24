#pragma once
#include <string>
#include <list>
#include <random>
#include <chrono>
#include <vector>
#include "Particle.h"
#include "BoundingBox.h"
#include "checkMemoryLeaks.h"

class ParticleGenerator {
protected:
	std::string name;
	Vector3 avrg_pos, avrg_vel;
	float avrg_lifeTime;
	int numParticles;
	std::vector<Particle*> models;
	
	inline void setParticle(Particle* model) { models.push_back(model); }
	ParticleGenerator(std::string name, const Vector3& genPos, const Vector3& averageVel, float averageLifeTime) : name(name), avrg_pos(genPos), avrg_vel(averageVel), avrg_lifeTime(averageLifeTime) {}

public:
	virtual std::list<Particle*> generateParticles() = 0;
	inline std::string getName() const { return name; }
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
	GaussianParticleGenerator(std::string name, const Point& generationPosition, const Vector3& sigmaPos, const Vector3& averageSpeed, const Vector3& sigmaSpeed, float averageLifeTime, float sigmaLifeTime);
	~GaussianParticleGenerator();
	std::list<Particle*> generateParticles() override;
};

class UniformParticleGenerator : public ParticleGenerator {
protected:
	std::default_random_engine gen;
	std::uniform_real_distribution<float>* vX;
	std::uniform_real_distribution<float>* vY;
	std::uniform_real_distribution<float>* vZ;
	std::uniform_real_distribution<float>* pX;
	std::uniform_real_distribution<float>* pY;
	std::uniform_real_distribution<float>* pZ;
	std::uniform_real_distribution<float>* t;
public:
	UniformParticleGenerator(std::string name, const Point& generationPosition, const Vector3& sigmaPos, const Vector3& averageSpeed, const Vector3& sigmaSpeed, float averageLifeTime, float sigmaLifeTime);
	~UniformParticleGenerator();
	std::list<Particle*> generateParticles() override;
};

