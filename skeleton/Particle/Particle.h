#pragma once
#include "../RenderUtils.hpp"
#include "../Generators/ForceGenerator.h"
#include "../checkMemoryLeaks.h"

class Particle {
public:
	enum class Particle_Type {NORMAL, PROJECTILE, FIREWORK};
	struct particle_data {
		Particle_Type type;
		Vector3 vel;
		float damping;
		physx::PxTransform pose;
		RenderItem* renderItem = nullptr;
		float lifeTime = 0.0f;
		float size;
		Vector4 colour;
		Force force;
		float inv_mass;
	};

	Particle(Particle& p);
	Particle(Particle_Type type, float damping = 0.998, Vector4 Col = { 1,0,0,1 });
	Particle(Particle_Type type, float inv_mass, Vector3 Pos, Vector3 Vel, float damping = 0.998, Vector4 Col = {1,0,0,1});
	Particle(Particle_Type type, float inv_mass, Vector3 Pos, Vector3 Vel, float size = 1.0f, float damping = 0.998, Vector4 Col = { 1,0,0,1 });
	virtual ~Particle();

	virtual void integrate(double t);
	inline physx::PxTransform const getPosition() const { return data.pose; }
	inline void setLifeTime(float lifeTime) { this->data.lifeTime = lifeTime; }
	inline double const getLifeTime() const {return data.lifeTime;}
	inline particle_data getData() const { return data; }
	inline void setData(const particle_data& data) { this->data = data; }
	inline void setPosition(Vector3 position) { this->data.pose = physx::PxTransform(position); }
	inline void setInverseMass(float inverse_mass) { this->data.inv_mass = inverse_mass; }
	inline void setVelocity(Vector3 velocity) { this->data.vel = velocity; }
	inline void addForce(const Force& f) { data.force += f; }
	inline double getParticleVolume() const { return (4.0 / 3) * (std::atan(1) * 4) * data.size/2 * data.size/2 * data.size/2; }
	inline double getParticleDensity() const { return getParticleVolume() * data.inv_mass; }

protected:
	particle_data data;

	inline void clearForce() { data.force *= 0.0; }
};
