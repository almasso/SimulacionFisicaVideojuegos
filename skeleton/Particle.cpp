#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 vel) {
	this->vel = vel;
	pose = physx::PxTransform(pos);
	physx::PxSphereGeometry sphere(10);
	physx::PxShape* shape = CreateShape(sphere);
	Vector4 col = { 255,0,0,1 };
	renderItem = new RenderItem(shape, &pose, col);
}

Particle::~Particle() {
	renderItem->release();
}

void Particle::integrate(double t) {
	pose.p += vel * t;
}