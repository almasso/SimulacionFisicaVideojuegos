#pragma once
#include "RenderUtils.hpp"
#include "checkMemoryLeaks.h"

class Particle {
public:
	Particle(Particle& p);
	Particle(float damping = 0.998, Vector4 Col = { 1,0,0,1 }, bool affectedByGravity = false);
	Particle(Vector3 Pos, Vector3 Vel, float damping = 0.998, Vector4 Col = {1,0,0,1}, bool affectedByGravity = false);
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acceleration, float damping = 0.998, Vector4 Col = {1,0,0,1}, bool affectedByGravity = false);
	Particle(Vector3 Pos, Vector3 Vel, Vector3 Acceleration, float size = 1.0f, float damping = 0.998, Vector4 Col = { 1,0,0,1 }, bool affectedByGravity = false);
	virtual ~Particle();

	virtual void integrate(double t);
	inline physx::PxTransform const getPosition() const { return pose; }
	inline double setLifeTime(float lifeTime) { this->lifeTime = lifeTime; }
	inline double const getLifeTime() const {return lifeTime;}
	inline void setPosition(Vector3 position) { this->pose = physx::PxTransform(position); }
	inline void setVelocity(Vector3 velocity) { this->vel = velocity; }

protected:
	Vector3 vel;
	Vector3 acceleration;
	float damping;
	physx::PxTransform pose;
	RenderItem* renderItem;
	float lifeTime = 0.0f;
	float size;
	Vector4 colour;

	inline void setAcceleration(Vector3 acceleration) { this->acceleration = acceleration; }
};
