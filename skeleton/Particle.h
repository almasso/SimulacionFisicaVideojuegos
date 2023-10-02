#pragma once
#include "RenderUtils.hpp"
#include "checkMemoryLeaks.h"

class Particle {
public:
	Particle(Vector3 Pos, Vector3 Vel, float damping = 0.998, Vector4 Col = {1,0,0,1});
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acceleration, float damping = 0.998, Vector4 Col = {1,0,0,1});
	virtual ~Particle();

	virtual void integrate(double t);
	inline physx::PxTransform const getPosition() const { return pose; }
	inline double const getStartTime() const { return startTime; }

protected:
	Vector3 vel;
	Vector3 acceleration;
	float damping;
	physx::PxTransform pose;
	RenderItem* renderItem;
	double startTime;

	inline void setAcceleration(Vector3 acceleration) { this->acceleration = acceleration; }
};

