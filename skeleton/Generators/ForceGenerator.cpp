#include "ForceGenerator.h"
#include "../Particle/Particle.h"
#include "../checkMemoryLeaks.h"
#include <math.h>
#include <numbers>
#include <iostream>
#include <string>
#include "../Messages/Message.h"

void GravityForceGenerator::updateForce(Particle* p, double duration) {
	if (fabs(p->getData().inv_mass) < 1e-10 || p->getData().type == Particle::Particle_Type::SOLID) return;
	p->addForce(_gravity * 1 / p->getData().inv_mass);
}

void ParticleDragGenerator::updateForce(Particle* p, double duration) {
	if (fabs(p->getData().inv_mass) < 1e-10 || !zone->isInBoundingBox(p->getPosition().p)) return;

	Vector3 v = p->getVelocity();
	Vector3 dragF = _k1 * (_windSpeed - v) + _k2 * (_windSpeed - v).magnitude() * (_windSpeed - v);
	p->addForce(dragF);
}

void WhirlpoolGenerator::updateForce(Particle* p, double duration) {
	if (fabs(p->getData().inv_mass) < 1e-10 || !zone->isInBoundingBox(p->getPosition().p)) return;

	Vector3 pos = p->getPosition().p;
	Vector3 vel = p->getVelocity();
	_whirlpoolSpeed = _k * Vector3(-(pos.z - _whirlpoolCenter.z), - (pos.y - _whirlpoolCenter.y), pos.x - _whirlpoolCenter.x);
	Vector3 force = _k1 * (_whirlpoolSpeed - vel);
	p->addForce(force);
}

Vector3 ExplosionGenerator::explosionSpeed = Vector3(100, 100, 100);

void ExplosionGenerator::updateForce(Particle* p, double duration) {
	//explosionRadius = explosionSpeed.magnitude() * _t;
	float r = (p->getPosition().p - explosionCenter).magnitude();
	if (fabs(p->getData().inv_mass) < 1e-10) return; 
	
	if( _t >= 4 * tau && !messageSent) {
		message::Message m((int)message::msgID::_m_GENERATOR_ERASABLE);
		m.genData.fg = this;
		message::MessageManager::sendMessage(m);
		messageSent = true;
		return;
	}

	Vector3 force = (K / (r * r)) * (p->getPosition().p - explosionCenter) * exp(-_t / tau);
	p->addForce(force);
}

void SpringForceGenerator::updateForce(Particle* p, double duration) {
	std::list<message::Message> msgs = message::MessageManager::receiveMessages((int)message::msgID::_m_CHANGE_HOOKE_CONSTANT);
	if (msgs.size() > 0) {
		_k = msgs.front().hookeValue.val;
	}

	Vector3 relative_pos_vector = _other->getPosition().p - p->getPosition().p;
	Vector3 force;

	const float length = relative_pos_vector.normalize();
	const float delta_x = length - _resting_length;

	force = relative_pos_vector * delta_x * _k;
	p->addForce(force);
}

AnchoredSpringFG::AnchoredSpringFG(double k, double resting, const Vector3& anchor_pos) : SpringForceGenerator(k, resting, nullptr) {
	_other = new Particle(Particle::Particle_Type::NORMAL, 0, anchor_pos, Vector3(0, 0, 0), 0, Vector4(0, 0, 0, 0));
}

BuoyancyForceGenerator::BuoyancyForceGenerator(float d, Vector3 pos) : _liquid_density(d) {
	_liquid_particle = new Plane(pos, Vector3(50, 0.5f, 50), Vector4(0, 0, 1, 1));
};

void BuoyancyForceGenerator::updateForce(Particle* p, double duration) {
	float h = p->getPosition().p.y;
	float h0 = _liquid_particle->getPos().p.y;
	float _height = p->getData().size;

	Vector3 f(0, 0, 0);
	float immersed = 0.0;
	if (h - h0 > _height * 0.5) immersed = 0.0;
	else if (h0 - h > _height * 0.5) immersed = 1.0;
	else immersed = (h0 - h) / _height + 0.5;
	f.y = _liquid_density * p->getParticleVolume() * immersed * _gravity;

	p->addForce(f);
}