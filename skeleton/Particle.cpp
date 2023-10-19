#include "Particle.h"
#include <iostream>
//#define DEBUG

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acceleration, float damping, Vector4 Col, bool affectedByGravity) : Particle(pos, vel, acceleration + Vector3(0.0f, (float)(-9.8f * affectedByGravity), 0), 1, damping, Col, false) {}

Particle::Particle(Vector3 pos, Vector3 vel, Vector3 acceleration, float size, float damping, Vector4 Col, bool affectedByGravity) : acceleration(acceleration + Vector3(0.0f, (float)(-9.8f * affectedByGravity), 0)), vel(vel), pose(physx::PxTransform(pos)), damping(damping) {
	physx::PxSphereGeometry sphere(size);
	physx::PxShape* shape = CreateShape(sphere);
	renderItem = new RenderItem(shape, &pose, Col);
	startTime = GetLastTime();
	lifeTime = 0.0f;
}

Particle::Particle(Vector3 pos, Vector3 vel, float damping, Vector4 col, bool affectedByGravity) : Particle(pos, vel, Vector3(0.0f, (float)(-9.8f * affectedByGravity), 0.0f), damping, col, false) {}

Particle::~Particle() {
	renderItem->release();
}

void Particle::integrate(double t) {
	pose.p += vel * t;
	vel += acceleration * t;
	vel *= pow(damping, t);

	lifeTime += t;

#ifdef DEBUG
	std::cout << "POSICIÓN:(" << pose.p.x << "," << pose.p.y << "," << pose.p.z << ")\n";
	std::cout << "VELOCIDAD:(" << vel.x << "," << vel.y << "," << vel.z << ")\n";
	std::cout << "ACELERACIÓN:(" << acceleration.x << "," << acceleration.y << "," << acceleration.z << ")\n";
#endif
}