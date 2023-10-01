#include "Particle.h"
#include <iostream>

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acceleration, float damping) : acceleration(acceleration), vel(vel), pose(physx::PxTransform(pos)), damping(damping) {
	physx::PxSphereGeometry sphere(1);
	physx::PxShape* shape = CreateShape(sphere);
	Vector4 col = { 255,0,0,1 };
	renderItem = new RenderItem(shape, &pose, col);
}

Particle::Particle(Vector3 pos, Vector3 vel, float damping) : Particle(pos, vel, Vector3(0.0f, 0.0f, 0.0f), damping) {}

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