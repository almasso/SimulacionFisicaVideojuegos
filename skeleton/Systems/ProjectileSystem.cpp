#include "ProjectileSystem.h"

ProjectileSystem::~ProjectileSystem() {
	for (auto p : _particles) delete p;
	for (auto f : _forces) delete f;
}

void ProjectileSystem::update(double t) {
	for (auto it = _particles.begin(); it != _particles.end();) {
		(*it)->integrate(t);

		if ((*it)->getLifeTime() >= tiempo_borrado_proj || !_bb.isInBoundingBox((*it)->getPosition().p)) {
			_particleRegistry.deleteParticleRegistry(*it);
			delete (*it);
			it = _particles.erase(it);
		}
		else ++it;
	}
	_particleRegistry.updateForces(t);
}

void ProjectileSystem::generateProjectile(Projectile* p) {
	_particleRegistry.addRegistry(p, _forces);
	_particles.push_back(p);
}