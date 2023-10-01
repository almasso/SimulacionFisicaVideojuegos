#pragma once
#include "RenderUtils.hpp"
#include "checkMemoryLeaks.h"

class Particle {
public:
	Particle(Vector3 Pos, Vector3 Vel, float damping = 0.998);
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acceleration, float damping = 0.998);
	virtual ~Particle();

	virtual void integrate(double t);

protected:
	Vector3 vel;
	Vector3 acceleration;
	float damping;
	physx::PxTransform pose;
	RenderItem* renderItem;

	inline void setAcceleration(Vector3 acceleration) { this->acceleration = acceleration; }
};

