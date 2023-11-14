#pragma once
#include <unordered_map>
#include <list>
#include "../Generators/ForceGenerator.h"
#include "../Particle/Particle.h"
#include "../checkMemoryLeaks.h"

class ParticleForceRegistry : public std::unordered_multimap<Particle*, ForceGenerator*> {
public:
	inline void updateForces(double duration) {
		for (auto it = begin(); it != end(); it++) {
			it->second->updateForce(it->first, duration);
		}
	}

	inline void addRegistry(Particle* p, ForceGenerator* fg) { this->insert({ p, fg }); }
	void addRegistry(Particle* p, std::vector<ForceGenerator*> fgs);
	void addRegistry(std::list<Particle*> ps, ForceGenerator* fg);
	void addRegistry(std::list<Particle*> ps, std::vector<ForceGenerator*> fgs);
	
	inline void deleteParticleRegistry(Particle* p) {
		auto it = this->find(p);
		while (it != this->end()) {
			this->erase(it);
			it = this->find(p);
		}
	}
};

