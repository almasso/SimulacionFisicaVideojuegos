#include "ParticleSystem.h"

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name) {
	for (auto it = _particle_generators.begin(); it != _particle_generators.end(); ++it) {
		if ((*it)->getName() == name) return (*it);
	}
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

	//_particles.push_back(new Particle(genPos, Vector3((*vX)(gen), (*vY)(gen), (*vZ)(gen)), 0.998f, Vector4(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1), true));
}