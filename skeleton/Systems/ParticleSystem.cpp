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
	for(auto e : _forces) e->updateTime(t);
	_particleRegistry.updateForces(t);


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
	
	std::list<message::Message> msgs = message::MessageManager::receiveMessages((int)message::msgID::_m_GENERATOR_ERASABLE);
	for (auto it = msgs.begin(); it != msgs.end(); ++it) {
		_particleRegistry.deleteInstancesOfForceGenerator(it->genData.fg);
		for (auto ot = _forces.begin(); ot != _forces.end();) {
			if ((*ot) == it->genData.fg) {
				auto e = (*ot);
				ot = _forces.erase(ot);
				delete e;
			}
			else ++ot;
		}
	}
}

void ParticleSystem::generateFirework(Vector3 genPos, Vector3 vel, int gen, float damping, Vector4 col) {
	Firework* tmp = new Firework(gen, genPos, vel, damping, col);
	_particleRegistry.addRegistry(tmp, _forces);
	_particles.push_back(tmp);
}

void ParticleSystem::generateExplosion(BoundingBox* bb) {
	addForceGenerator(new ExplosionGenerator(bb->center(), 200000, 0.5f));
	_particleRegistry.addRegistry(_particles, _forces[_forces.size() - 1]);
}

void ParticleSystem::generateSpringDemo() {
	Particle* p1 = new Particle(Particle::Particle_Type::NORMAL, 1, Vector3(0, 80, 0), Vector3(0, 0, 0), 0.998, Vector4(1,1,0,1));
	Particle* p2 = new Particle(Particle::Particle_Type::NORMAL, 1, Vector3(10, 160, 0), Vector3(0, 0, 0), 0.998, Vector4(0, 1, 0, 1));
	SpringForceGenerator* f1 = new SpringForceGenerator(1, 30, p2);
	SpringForceGenerator* f2 = new SpringForceGenerator(1, 30, p1);
	_particleRegistry.addRegistry(p1, f1);
	_particleRegistry.addRegistry(p2, f2);
	_forces.push_back(f1);
	_forces.push_back(f2);
	_particles.push_back(p1);
	_particles.push_back(p2);
}

void ParticleSystem::generateAnchoredSpringDemo() {
	Particle* p3 = new Particle(Particle::Particle_Type::NORMAL, 1, Vector3(0, 50, 0), Vector3(0, 0, 0), 0.998, Vector4(0, 1, 1, 1));
	AnchoredSpringFG* f3 = new AnchoredSpringFG(1, 30, { 0,60,0 });
	_particleRegistry.addRegistry(p3, f3);
	_forces.push_back(f3);
	_particles.push_back(p3);
}

void ParticleSystem::generateBuoyancyDemo() {
	Particle* p4 = new Particle(Particle::Particle_Type::NORMAL, 0.001f, Vector3(-20, 50, 0), Vector3(0, 0, 0), 4, 0.6, Vector4(0.5, 0.5, 1, 1));
	Particle* p5 = new Particle(Particle::Particle_Type::NORMAL, 0.02f, Vector3(-10, 50, 0), Vector3(0, 0, 0),3, 0.6, Vector4(0.5, 0.5, 1, 1));
	Particle* p6 = new Particle(Particle::Particle_Type::NORMAL, 0.3f, Vector3(10, 50, 0), Vector3(0, 0, 0), 2, 0.6, Vector4(0.5, 0.5, 1, 1));
	Particle* p7 = new Particle(Particle::Particle_Type::NORMAL, 0.7f, Vector3(20, 50, 0), Vector3(0, 0, 0), 0.6, Vector4(0.5, 0.5, 1, 1));
	BuoyancyForceGenerator* f4 = new BuoyancyForceGenerator(1000, Vector3(0,20,0));
	_particleRegistry.addRegistry(p4, f4);
	_particleRegistry.addRegistry(p5, f4);
	_particleRegistry.addRegistry(p6, f4);
	_particleRegistry.addRegistry(p7, f4);
	_forces.push_back(f4);
	_particles.push_back(p4);
	_particles.push_back(p5);
	_particles.push_back(p6);
	_particles.push_back(p7);
}