#include "ParticleSystem.h"
#include "checkMemoryLeaks.h"

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name) {
	for (auto it = _particle_generators.begin(); it != _particle_generators.end(); ++it) {
		if ((*it)->getName() == name) return (*it);
	}
}

ParticleSystem::~ParticleSystem() {
	for (auto p : _particles) delete p;
	for (auto g : _particle_generators) delete g;
}

void ParticleSystem::update(double t) {
	for (auto it = _particles.begin(); it != _particles.end();) {
		(*it)->integrate(t);
		
		if ((*it)->getLifeTime() >= tiempo_borrado || !_bb.isInBoundingBox((*it)->getPosition().p)) {
			delete (*it);
			it = _particles.erase(it);
		}
		else ++it;
	}

	for (auto at = _particle_generators.begin(); at != _particle_generators.end(); ++at) {
		_particles.splice(_particles.end(), (*at)->generateParticles(1));
	}
}