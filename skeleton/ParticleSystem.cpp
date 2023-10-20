#include "ParticleSystem.h"
ParticleSystem::ParticleSystem(const BoundingBox& bB, const Point& genPos, Vector3 avgSpeed, Vector3 sigma) : _bb(bB), genPos(genPos), gen(std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count())) {
	vX = new std::normal_distribution<float>(avgSpeed.x, sigma.x);
	vY = new std::normal_distribution<float>(avgSpeed.y, sigma.y);
	vZ = new std::normal_distribution<float>(avgSpeed.z, sigma.z);
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