#pragma once
#include <unordered_map>
#include "../Generators/ForceGenerator.h"
#include "../Particle/Particle.h"

class ParticleForceRegistry : public std::unordered_multimap<Particle*, ForceGenerator*> {
public:
	inline void updateForces(double duration) {
		for (auto it = begin(); it != end(); it++) {
			it->second->updateForce(it->first, duration);
		}
	}

	inline void addRegistry(Particle* p, ForceGenerator* fg) { this->insert({ p, fg }); }
	inline void deleteParticleRegistry(Particle* p) {
		auto it = this->find(p);
		while (it != this->end()) {
			this->erase(it);
			it = this->find(p);
		}
	}
};

