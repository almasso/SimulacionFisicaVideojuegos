#include "ParticleSystem.h"
void ParticleSystem::update(double t) {
	for (auto p : _particles) {
		p->integrate(t);
		/*
		if(getLifeTime >= ciertoNumero) 
		*/
		
	}
}