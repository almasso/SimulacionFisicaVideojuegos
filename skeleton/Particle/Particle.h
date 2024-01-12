#pragma once
#include "../RenderUtils.hpp"
#include "../Generators/ForceGenerator.h"
#include "../checkMemoryLeaks.h"

class ParticleGenerator;

class Particle {
public:
	enum class Particle_Type {NORMAL, PROJECTILE, FIREWORK, SOLID};
	enum class Particle_Shape {SPHERE, CUBE, MIX};
	struct particle_data {
		Particle_Type type;
		Particle_Shape shape;
		Vector3 vel;
		float damping;
		physx::PxTransform pose;
		RenderItem* renderItem = nullptr;
		bool affectedByLifetime;
		float lifeTime = 0.0f;
		float size;
		Vector4 colour;
		Force force;
		float inv_mass;
		ParticleGenerator* generator = nullptr;
		bool erasable = false;
	};

	Particle(Particle& p);
	Particle(Particle_Type type, Particle_Shape shape, float damping = 0.998, Vector4 Col = { 1,0,0,1 });
	Particle(Particle_Type type, Particle_Shape shape, float inv_mass, Vector3 Pos, Vector3 Vel, float damping = 0.998, Vector4 Col = {1,0,0,1});
	Particle(Particle_Type type, Particle_Shape shape, float inv_mass, Vector3 Pos, Vector3 Vel, float size = 1.0f, float damping = 0.998, Vector4 Col = { 1,0,0,1 });
	~Particle();

	virtual void integrate(double t);
	virtual inline physx::PxTransform const getPosition() const { return data.pose; }
	virtual inline void setLifeTime(float lifeTime) { this->data.lifeTime = lifeTime; }
	virtual inline double const getLifeTime() const {return data.lifeTime;}
	virtual inline particle_data getData() const { return data; }
	virtual inline void setData(const particle_data& data) { this->data = data; }
	virtual inline void setPosition(Vector3 position) { this->data.pose = physx::PxTransform(position); }
	virtual inline void setPosition(physx::PxTransform transform) { this->data.pose = transform; }
	virtual inline void setInverseMass(float inverse_mass) { this->data.inv_mass = inverse_mass; }
	virtual inline void setVelocity(Vector3 velocity) { this->data.vel = velocity; }
	virtual inline void addForce(const Force& f) { data.force += f; }
	virtual inline double getParticleVolume() const { 
		switch (this->data.shape) {
		case Particle_Shape::SPHERE: return (4.0 / 3) * (std::atan(1) * 4) * data.size / 2 * data.size / 2 * data.size / 2; break;
		case Particle_Shape::CUBE: return this->data.size * this->data.size * this->data.size; break;
		}
	}
	virtual inline double getParticleDensity() const { return (1 / data.inv_mass) / getParticleVolume(); }
	virtual inline void setParticleGenerator(ParticleGenerator* pG) { this->data.generator = pG; }
	virtual inline ParticleGenerator* getParticleGenerator() const { return this->data.generator; }
	virtual inline Vector3 getVelocity() const { return data.vel; }
	virtual inline bool isErasable() const { return data.erasable; }
	virtual inline void setErasable() { data.erasable = true; }

protected:
	particle_data data;

	inline void clearForce() { data.force *= 0.0; }
};

class SolidParticle : public Particle {
private:
	physx::PxRigidActor* esfera;
public:
	SolidParticle(physx::PxPhysics* gPhysics, physx::PxScene* gScene, SolidParticle& p);
	SolidParticle(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Particle_Type type, Particle_Shape shape, float damping = 0.998, Vector4 Col = { 1,0,0,1 });
	SolidParticle(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Particle_Type type, Particle_Shape shape, float inv_mass, Vector3 Pos, Vector3 Vel, float damping = 0.998, Vector4 Col = { 1,0,0,1 });
	SolidParticle(physx::PxPhysics* gPhysics, physx::PxScene* gScene, Particle_Type type, Particle_Shape shape, float inv_mass, Vector3 Pos, Vector3 Vel, float size = 1.0f, float damping = 0.998, Vector4 Col = { 1,0,0,1 });
	~SolidParticle();
	
	void integrate(double time) override;
	inline void addForce(const Force& f) override { static_cast<physx::PxRigidDynamic*>(esfera)->addForce(f); }
	inline void addForce(const Force& f, physx::PxForceMode::Enum mode) { static_cast<physx::PxRigidDynamic*>(esfera)->addForce(f, mode); }
	inline physx::PxTransform const getPosition() const override { return static_cast<physx::PxRigidDynamic*>(esfera)->getGlobalPose();}
	inline physx::PxRigidActor* getRigidActor() const { return esfera; }
	inline void setPosition(physx::PxTransform transform) override { this->data.pose = transform; esfera->setGlobalPose(transform); }
	inline Vector3 getVelocity() const override { return static_cast<physx::PxRigidDynamic*>(esfera)->getLinearVelocity();}
};
