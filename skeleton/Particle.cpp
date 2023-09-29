#include "Particle.h"
#include <iostream>

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acceleration, float damping) : acceleration(acceleration), vel(vel), pose(physx::PxTransform(pos)), damping(damping) {
	physx::PxSphereGeometry sphere(10);
	physx::PxShape* shape = CreateShape(sphere);
	Vector4 col = { 255,0,0,1 };
	renderItem = new RenderItem(shape, &pose, col);
}

Particle::Particle(Vector3 pos, Vector3 vel, float damping) : Particle(pos, vel, Vector3(0, 0, 0), damping) {}

Particle::~Particle() {
	renderItem->release();
}

void Particle::integrate(double t) {
	pose.p += vel * t;
	vel += acceleration * t;
	vel *= pow(damping, t);
}