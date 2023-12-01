#pragma once
#include <list>
#include <string>
#include <vector>
#include "../Messages/Message.h"
#include "../Particle/Particle.h"
#include "../Particle/Firework.h"
#include "../Utils/BoundingBox.h"
#include "../Generators/ParticleGenerator.h"
#include "../Registries/ParticleForceRegistry.h"
#include "../Generators/ForceGenerator.h"
#include "../checkMemoryLeaks.h"

const float tiempo_borrado = 40.0f;
const float tiempo_fireworks = 5.0f;

class ParticleSystem {
protected:
	std::list<Particle*> _particles;
	std::list<ParticleGenerator*> _particle_generators;
	ParticleForceRegistry _particleRegistry;
	std::vector<ForceGenerator*> _forces;

	BoundingBox _bb;
	bool explosionGenerated = false;
	
public:
	ParticleSystem(const BoundingBox& bb) : _bb(bb) {}
	~ParticleSystem();
	void update(double t);
	inline void addParticleGenerator(ParticleGenerator* pG) { _particle_generators.push_back(pG); };
	inline void addForceGenerator(ForceGenerator* fG) { _forces.push_back(fG); _particleRegistry.addRegistry(_particles, fG); }
	ParticleGenerator* getParticleGenerator(std::string name);
	void generateFirework(Vector3 genPos, Vector3 vel, int gen = 4, float damping = 0.998f, Vector4 col = { 0,1,0,1 });
	void generateExplosion(BoundingBox* bb);
	void generateSpringDemo();
	void generateAnchoredSpringDemo();
	void generateBuoyancyDemo();
};

