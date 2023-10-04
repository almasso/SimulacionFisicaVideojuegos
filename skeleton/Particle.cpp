#include "Particle.h"
#include <iostream>

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acceleration, float damping, Vector4 Col) : Particle(pos, vel, acceleration, 1, damping, Col) {}

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acceleration, float size, float damping, Vector4 Col) : acceleration(acceleration), vel(vel), pose(physx::PxTransform(pos)), damping(damping) {
	physx::PxSphereGeometry sphere(size);
	physx::PxShape* shape = CreateShape(sphere);
	renderItem = new RenderItem(shape, &pose, Col);
	startTime = GetLastTime();
}

Particle::Particle(Vector3 pos, Vector3 vel, float damping, Vector4 col) : Particle(pos, vel, Vector3(0.0f, 0.0f, 0.0f), damping, col) {}

Particle::~Particle() {
	renderItem->release();
}

void Particle::integrate(double t) {
	pose.p += vel * t;
	vel += acceleration * t;
	vel *= pow(damping, t);

#ifdef DEBUG
	std::cout << "POSICIÓN:(" << pose.p.x << "," << pose.p.y << "," << pose.p.z << ")\n";
	std::cout << "VELOCIDAD:(" << vel.x << "," << vel.y << "," << vel.z << ")\n";
	std::cout << "ACELERACIÓN:(" << acceleration.x << "," << acceleration.y << "," << acceleration.z << ")\n";
#endif
}