#include "ParticleForceRegistry.h"
#include "../checkMemoryLeaks.h"

void ParticleForceRegistry::addRegistry(std::list<Particle*> ps, ForceGenerator* fg) {
	int tmp = ps.size();
	for (int i = 0; i < tmp; ++i) {
		this->insert({ ps.front(), fg });
		ps.pop_front();
	}
}

void ParticleForceRegistry::addRegistry(std::list<Particle*> ps, std::vector<ForceGenerator*> fgs) {
	int tmp = ps.size();
	for (int i = 0; i < tmp; ++i) {
		for (int j = 0; j < fgs.size(); ++j) {
			this->insert({ ps.front(), fgs[j]});
		}
		ps.pop_front();
	}
}

void ParticleForceRegistry::addRegistry(Particle* p, std::vector<ForceGenerator*> fgs) {
	for (int i = 0; i < fgs.size(); ++i) {
		this->insert({ p, fgs[i] });
	}
}