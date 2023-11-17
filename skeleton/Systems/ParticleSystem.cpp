#include "ParticleSystem.h"
#include "../checkMemoryLeaks.h"

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name) {
	for (auto it = _particle_generators.begin(); it != _particle_generators.end(); ++it) {
		if ((*it)->getName() == name) return (*it);
	}
}

ParticleSystem::~ParticleSystem() {
	for (auto p : _particles) delete p;
	for (auto g : _particle_generators) delete g;
	for (auto f : _forces) delete f;
}

void ParticleSystem::update(double t) {
	for (auto it = _particles.begin(); it != _particles.end();) {
		(*it)->integrate(t);
		
		if ((((*it)->getData().type == Particle::Particle_Type::FIREWORK) && (*it)->getLifeTime() >= tiempo_fireworks)) {
			std::list<Particle*> fwks = static_cast<Firework*>((*it))->explode();
			_particleRegistry.addRegistry(fwks, _forces);
			_particles.splice(_particles.end(), fwks);
			_particleRegistry.deleteParticleRegistry(*it);
			delete (*it);
			it = _particles.erase(it);
		}
		else if ((*it)->getLifeTime() >= tiempo_borrado || !_bb.isInBoundingBox((*it)->getPosition().p)) {
			_particleRegistry.deleteParticleRegistry(*it);
			delete (*it);
			it = _particles.erase(it);
		}
		else ++it;
	}

	for (auto at = _particle_generators.begin(); at != _particle_generators.end(); ++at) {
		std::list<Particle*> ptcls = (*at)->generateParticles(1);
		_particleRegistry.addRegistry(ptcls, _forces);
		_particles.splice(_particles.end(), ptcls);
	}
	for (auto f : _forces) f->updateTime(t);
	_particleRegistry.updateForces(t);
}

void ParticleSystem::generateFirework(Vector3 genPos, Vector3 vel, int gen, float damping, Vector4 col) {
	Firework* tmp = new Firework(gen, genPos, vel, damping, col);
	_particleRegistry.addRegistry(tmp, _forces);
	_particles.push_back(tmp);
}

void ParticleSystem::generateExplosion(BoundingBox* bb) {
	addForceGenerator(new ExplosionGenerator(bb->center(), 200000, 1));
	_particleRegistry.addRegistry(_particles, _forces[_forces.size() - 1]);
}