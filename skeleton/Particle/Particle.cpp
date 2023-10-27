#include "Particle.h"
#include "../checkMemoryLeaks.h"
#include <iostream>
//#define DEBUG

Particle::Particle(Particle& p) {
	this->data.type = p.data.type;
	this->data.vel = p.data.vel;
	this->data.acceleration = p.data.acceleration;
	this->data.damping = p.data.damping;
	this->data.pose = p.data.pose;
	this->data.lifeTime = 0.0;
	this->data.size = p.data.size;
	this->data.colour = p.data.colour;
	
	physx::PxSphereGeometry sphere(this->data.size);
	physx::PxShape* shape = CreateShape(sphere);
	this->data.renderItem = new RenderItem(shape, &(this->data.pose), this->data.colour);
}

Particle::Particle(Particle_Type type, float damping, Vector4 Col) {
	data.type = type;
	data.damping = damping;
	data.colour = Col;
}

Particle::Particle(Particle_Type type, Vector3 pos, Vector3 vel, float damping, Vector4 col, bool affectedByGravity) : Particle(type, pos, vel, Vector3(0.0f, (float)(-9.8f * affectedByGravity), 0.0f), damping, col, false) {}

Particle::Particle(Particle_Type type, Vector3 pos, Vector3 vel, Vector3 acceleration, float damping, Vector4 Col, bool affectedByGravity) : Particle(type, pos, vel, acceleration + Vector3(0.0f, (float)(-9.8f * affectedByGravity), 0), 1, damping, Col, false) {}

Particle::Particle(Particle_Type type, Vector3 pos, Vector3 vel, Vector3 acceleration, float size, float damping, Vector4 Col, bool affectedByGravity) {
	data.type = type;
	data.acceleration = acceleration + Vector3(0.0f, (float)(-9.8f * affectedByGravity), 0);
	data.vel = vel;
	data.pose = physx::PxTransform(pos);
	data.damping = damping;
	data.size = size;
	data.colour = Col;
	physx::PxSphereGeometry sphere(this->data.size);
	physx::PxShape* shape = CreateShape(sphere);
	data.renderItem = new RenderItem(shape, &data.pose, Col);
	data.lifeTime = 0.0f;
}

Particle::~Particle() {
	if(data.renderItem != nullptr) data.renderItem->release();
}

void Particle::integrate(double t) {
	//Euler semi-implícito
	data.vel += data.acceleration * t;
	data.pose.p += data.vel * t;
	data.vel *= pow(data.damping, t);

	data.lifeTime += t;

#ifdef DEBUG
	std::cout << "POSICIÓN:(" << pose.p.x << "," << pose.p.y << "," << pose.p.z << ")\n";
	std::cout << "VELOCIDAD:(" << vel.x << "," << vel.y << "," << vel.z << ")\n";
	std::cout << "ACELERACIÓN:(" << acceleration.x << "," << acceleration.y << "," << acceleration.z << ")\n";
#endif
}