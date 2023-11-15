#include "ForceGenerator.h"
#include "../Particle/Particle.h"
#include "../checkMemoryLeaks.h"
#include <math.h>

void GravityForceGenerator::updateForce(Particle* p, double duration) {
	if (fabs(p->getData().inv_mass) < 1e-10) return;
	p->addForce(_gravity * 1 / p->getData().inv_mass);
}

void ParticleDragGenerator::updateForce(Particle* p, double duration) {
	if (fabs(p->getData().inv_mass) < 1e-10) return;

	Vector3 v = p->getData().vel;
	Vector3 dragF = _k1 * (_windSpeed - v) + _k2 * (_windSpeed - v).magnitude() * (_windSpeed - v);
	p->addForce(dragF);
}

void WhirlpoolGenerator::updateForce(Particle* p, double duration) {
	if (fabs(p->getData().inv_mass) < 1e-10) return;

	Vector3 pos = p->getData().pose.p;
	Vector3 vel = p->getData().vel;
	_whirlpoolSpeed = _k * Vector3(-(pos.z - _whirlpoolCenter.z), - (pos.y - _whirlpoolCenter.y), pos.x - _whirlpoolCenter.x);
	Vector3 force = _k1 * (_whirlpoolSpeed - vel);
	p->addForce(force);
}
