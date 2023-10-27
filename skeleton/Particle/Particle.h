#pragma once
#include "../RenderUtils.hpp"
#include "../checkMemoryLeaks.h"

class Particle {
public:
	enum class Particle_Type {NORMAL, PROJECTILE, FIREWORK};
	struct particle_data {
		Particle_Type type;
		Vector3 vel;
		Vector3 acceleration;
		float damping;
		physx::PxTransform pose;
		RenderItem* renderItem = nullptr;
		float lifeTime = 0.0f;
		float size;
		Vector4 colour;
	};

	Particle(Particle& p);
	Particle(Particle_Type type, float damping = 0.998, Vector4 Col = { 1,0,0,1 });
	Particle(Particle_Type type, Vector3 Pos, Vector3 Vel, float damping = 0.998, Vector4 Col = {1,0,0,1}, bool affectedByGravity = false);
	Particle(Particle_Type type, Vector3 Pos, Vector3 Vel, Vector3 Acceleration, float damping = 0.998, Vector4 Col = {1,0,0,1}, bool affectedByGravity = false);
	Particle(Particle_Type type, Vector3 Pos, Vector3 Vel, Vector3 Acceleration, float size = 1.0f, float damping = 0.998, Vector4 Col = { 1,0,0,1 }, bool affectedByGravity = false);
	virtual ~Particle();

	virtual void integrate(double t);
	inline physx::PxTransform const getPosition() const { return data.pose; }
	inline void setLifeTime(float lifeTime) { this->data.lifeTime = lifeTime; }
	inline double const getLifeTime() const {return data.lifeTime;}
	inline particle_data getData() const { return data; }
	inline void setData(const particle_data& data) { this->data = data; }
	inline void setPosition(Vector3 position) { this->data.pose = physx::PxTransform(position); }
	inline void setVelocity(Vector3 velocity) { this->data.vel = velocity; }

protected:
	particle_data data;

	inline void setAcceleration(Vector3 acceleration) { this->data.acceleration = acceleration; }
};
